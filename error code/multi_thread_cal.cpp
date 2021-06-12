
#include <thread>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>


template<typename T>
class Matrix{
    private:
        T** m   = nullptr;
        int row = 0;
        int col = 0;
    
    public:
        
        Matrix(){}
        Matrix(const int& row,const int& col){
            this->row = row;
            this->col = col;

            m = new T*[row];
            for(int i=0;i<row;i++){
                m[i] = new T[col]{};
            }
        }
        Matrix(const cv::Mat& img){
            row = img.rows;
            col = img.cols;

            m = new T*[row];
            for(int i=0;i<row;i++){
                m[i] = new T[col];
            }
            for(int i=0;i<row;i++)
                for(int j=0;j<col;j++){
                    m[i][j] = T(img.at<uchar>(i,j));
                }
            std::cout<<"constructor from Mat image"<<std::endl;            
        }
        ~Matrix(){
            for(int i=0;i<row;i++){
                if(m[i]!=nullptr){
                    delete[] m[i];
                    m[i] = nullptr; 
                }
            }
            if(m!=nullptr){
                delete[] m;
                m = nullptr;
            }
        }
        Matrix(const Matrix& rhs){
            row = rhs.row;
            col = rhs.col;

            m = new T*[row];
            for(int i=0;i<row;i++){
                m[i] = new T[col];
            }

            for(int i=0;i<row;i++)
                for(int j=0;j<col;j++){
                    m[i][j] = rhs.m[i][j];
                }
            std::cout<<"copy constructor"<<std::endl;
        }
        Matrix(Matrix&& rhs){
            row = rhs.row;
            col = rhs.col;
            m = std::move(rhs.m);
            
            //move后，将source内元素置空
            rhs.row = 0;
            rhs.col = 0;
            rhs.m = nullptr;
            rhs.~Matrix();

            std::cout<<"move constructor"<<std::endl;
        }
        friend void swap(Matrix& first,Matrix& second){
            std::swap(first.m,second.m);
            std::swap(first.row,second.row);
            std::swap(first.col,second.col);
        }
        Matrix& operator=(Matrix&& rhs){
            if(this->m != rhs.m){
                Matrix temp(std::move(rhs));
                swap(*this,temp);
            }                                   
            std::cout<<"move assignment"<<std::endl;
            return *this;           
        }
        // deep copy
        Matrix& operator=(const Matrix& rhs){
            Matrix temp(rhs);
            swap(*this,temp);
            std::cout<<"copy assignment"<<std::endl;
            return *this;
        }

        void shallowCopy(const Matrix& rhs){
            Matrix temp;
            swap(temp,*this);
            this->m = rhs.m;
            this->row = rhs.row;
            this->col = rhs.col;
        }

        bool DimensionMatch(const Matrix& lhs, const Matrix& rhs){
            return ((lhs.row == rhs.row) && (lhs.col == rhs.col));
        }
        //一维下标操作矩阵
        // T& operator[](int index){
        //     return m[index/col][index%col];
        // }
        //二维下标重载
        T* operator[](int row){
            return m[row];
        }
        int getRow() const{
            return this->row;
        }
        int getCol() const{
            return this->col;
        }

        Matrix& operator+=(const Matrix& rhs){
            if(!DimensionMatch((*this),rhs))
                std::cout<<"dimension not match. can't add."<<std::endl;
            for(int i=0;i<row;i++)
                for(int j=0;j<col;j++){
                    m[i][j] += rhs.m[i][j];
                }
            return *this;
        }
/*-----------------------------------------------------------------------------------------------------------------------------------
RVO返回值优化.函数内局部变量result返回，在以前我是避免这种做法的，只采用指针或者引用传递自定义类型，以为局部变量无法返回。但C++似乎可以返回局部变量，
通过编译器优化。继续解释，形如A=A+B，A、B都是矩阵,假设A、B都是左值，按常理A+B返回的是一个右值，然后赋值给A。实际编译器不会这样做，赋值部分直接采用
指针，不会调用赋值运算符
-------------------------------------------------------------------------------------------------------------------------------------*/
        friend Matrix operator+(const Matrix& lhs, const Matrix& rhs){
            Matrix result(lhs);
            result += rhs;
            std::cout<<"two lvalue add"<<std::endl;
            return result;
        }

        friend Matrix operator+(Matrix&& lhs,const Matrix& rhs){
            Matrix result(std::move(lhs));
            result += rhs;
            std::cout<<"left rvalue add right lvalue"<<std::endl;
            return result;
        }

        friend Matrix operator+(const Matrix& lhs, Matrix&& rhs){
            std::cout<<"left lvalue add right rvalue"<<std::endl;
            return std::move(rhs) + lhs;//调用上面的operator+
        }
};

template<typename T>
class parallel_smooth{
    private:
        Matrix<T> original;
        Matrix<T> result;
        int row;
        int col;
        int smooth_size = 10;
        //存储计算中间状态矩阵和所有线程
        std::vector<Matrix<T>> intermediate;
        std::vector<std::thread> threads;
        int block_size;//每个线程处理行数

        int num_threads;

        int calculate_threads(int length){
            if(length == 0)
                std::cout << "image reading error" << std::endl;
            
            const int min_per_thread = 250;
            const int max_threads = (length + min_per_thread - 1)/min_per_thread;
            const int hardware_threads = std::thread::hardware_concurrency();
            const int num_threads = std::min(hardware_threads!=0?hardware_threads:2,max_threads);
            //如果hardware_threads返回0.则使用2核
            return num_threads;
        }
/*————————————————————————————————————————————————————————————————————————————————————————————————————————
对图像上每一点做平滑处理，将值平均到smooth_size为半径(正方形半边长，当前点为正方形中心)的区域.
对边界区域依然如此，忽略图像矩阵外部分(如正方形大小为100像素，则平均时按照100，但丢弃超出矩阵边界的数据)
——————————————————————————————————————————————————————————————————————————————————————————————————————————*/
        void update_smooth_each_point(int row_position, int col_position, Matrix<T>& buffer){
            int row_starting = std::max(row_position - smooth_size,0);  //起始点不可小于0
            int row_ending   = std::min(row_position + smooth_size,row);//结束点不可大于最大行数
            int col_starting = std::max(col_position - smooth_size,0);  //起始点不可小于0
            int col_ending   = std::min(col_position + smooth_size,col);//结束点不可大于最大列数
            
            T smoothed_value = T(original[row_position][col_position])/((smooth_size+1)*(smooth_size+1));
            for(int i = row_starting;i<row_ending;i++)
                for(int j = col_starting;j<col_ending;j++){
                    buffer[i][j] = buffer[i][j] + smoothed_value;
                }
        }

        void update_smooth_each_block(int starting_row, int ending_row,Matrix<T>& buffer){
            for(int i=starting_row;i<ending_row;i++)
                for(int j=0;j<col;j++){
                    update_smooth_each_point(i,j,buffer);
                }
        }

/*---------------------------------------------------------------
按行分割到各线程，每个线程至少处理500行. 按照普遍计算机不高于8核来算，可并行总行数为4000. 
-----------------------------------------------------------------*/

    public:
        parallel_smooth(const Matrix<T>& input){
            row = input.getRow();
            col = input.getCol();
            num_threads = calculate_threads(row);
            original.shallowCopy(input);

            Matrix<T> temp(row,col);
            swap(result,temp);

            block_size = row/num_threads;
            std::vector<Matrix<T>> temp_intermediate(num_threads); 
            for(int i=0;i<num_threads;i++){
                Matrix<T> temp(row,col);
                swap(temp_intermediate[i],temp);
            }
            std::vector<std::thread> temp_threads(num_threads-1);
            intermediate.swap(temp_intermediate);
            threads.swap(temp_threads);
        }
        parallel_smooth(parallel_smooth& rhs) = delete;
        parallel_smooth& operator=(parallel_smooth& rhs) = delete;

        Matrix<T> getSmoothed(){
            int staring_row = 0;
            int ending_row  = 0;
            for(int i=0;i<num_threads-1;i++){
                ending_row += (i+1)*block_size;
                threads[i]  = std::thread(update_smooth_each_block,staring_row,ending_row,std::ref(intermediate[i]));
                staring_row = ending_row;
            }
            ending_row = row;//到达最后一行
            update_smooth_each_block(staring_row,ending_row,intermediate[num_threads-1]);
            std::for_each(threads.begin(),threads.end(),std::mem_fn(&std::thread::join));
            
            for(int i=0;i<num_threads;i++)
                result = std::move(result) + intermediate[i];

            return result;
        }
        
};

// int calculate_threads(int length){
//     if(length == 0)
//         std::cout << "image reading error" << std::endl;
    
//     const int min_per_thread = 250;
//     const int max_threads = (length + min_per_thread - 1)/min_per_thread;
//     const int hardware_threads = std::thread::hardware_concurrency();
//     const int num_threads = std::min(hardware_threads!=0?hardware_threads:2,max_threads);
//     //如果hardware_threads返回0.则使用2核
//     return num_threads;
// }

// template<typename T>
// void update_smooth_each_point(int row_position, int col_position, Matrix<T>& buffer,T smooth_size,int row,int col, Matrix<T>& original){
//     int row_starting = std::max(row_position - smooth_size,0);  //起始点不可小于0
//     int row_ending   = std::min(row_position + smooth_size,row);//结束点不可大于最大行数
//     int col_starting = std::max(col_position - smooth_size,0);  //起始点不可小于0
//     int col_ending   = std::min(col_position + smooth_size,col);//结束点不可大于最大列数
    
//     T smoothed_value = T(original[row_position][col_position])/((smooth_size+1)*(smooth_size+1));
//     for(int i = row_starting;i<row_ending;i++)
//         for(int j = col_starting;j<col_ending;j++){
//             buffer[i][j] = buffer[i][j] + smoothed_value;
//         }
// }

// template<typename T>
// void update_smooth_each_block(int starting_row, int ending_row,Matrix<T>& buffer, T smooth_size,int row,int col, Matrix<T>& original){
//     for(int i=starting_row;i<ending_row;i++)
//         for(int j=0;j<col;j++){
//             update_smooth_each_point(i,j,buffer,smooth_size,row,col,original);
//         }
// }

// template<typename T>
// void parallel(const Matrix<T>& input){
//     Matrix<T> original;
//     Matrix<T> result;
//     int row;
//     int col;
//     int smooth_size = 10;
//     //存储计算中间状态矩阵和所有线程
//     std::vector<Matrix<T>> intermediate;
//     std::vector<std::thread> threads;
//     int block_size;//每个线程处理行数

//     int num_threads;

//     row = input.getRow();
//     col = input.getCol();
//     num_threads = calculate_threads(row);
//     original.shallowCopy(input);

//     Matrix<T> temp(row,col);
//     swap(result,temp);

//     block_size = row/num_threads;
//     std::vector<Matrix<T>> temp_intermediate(num_threads); 
//     for(int i=0;i<num_threads;i++){
//         Matrix<T> temp(row,col);
//         swap(temp_intermediate[i],temp);
//     }
//     std::vector<std::thread> temp_threads(num_threads-1);
//     intermediate.swap(temp_intermediate);
//     threads.swap(temp_threads);

//     int staring_row = 0;
//     int ending_row  = 0;
//     for(int i=0;i<num_threads-1;i++){
//         ending_row += (i+1)*block_size;
//         threads[i]  = std::thread(update_smooth_each_block,staring_row,ending_row,std::ref(intermediate[i]),smooth_size,row,col,original);
//         staring_row = ending_row;
//     }
//     ending_row = row;//到达最后一行
//     update_smooth_each_block(staring_row,ending_row,intermediate[num_threads-1],smooth_size,row,col,original);
//     std::for_each(threads.begin(),threads.end(),std::mem_fn(&std::thread::join));
    
//     for(int i=0;i<num_threads;i++)
//         result = std::move(result) + intermediate[i];

// }


int main()
{
    /*
    Mat img=imread("D:/pyfiles/wsc/1.jpg");
    Mat dst;
    Size dsize = Size(1000,1000);
    resize(img,dst,dsize);
    Mat channel[3];
    split(img,channel);

    Matrix<float> test(channel[0]);
    Matrix<float> test1(channel[1]);
    Matrix<float> test2(channel[2]);
    */
/*
    Matrix<float> A=std::move(test);//不会调用move assignment,调用的是move constructor
    Matrix<float> B;
    B = std::move(test);//这才会调用move assignment

    int row = test.getRow();
    int col = test.getCol();
    Matrix<float> result(row,col);
    
    result = std::move(result) + test;
    result = std::move(result) + test1;
    result = std::move(result) + test2;
*/
    Matrix<float> test(5000,4000);
    parallel_smooth<float> cal(test);
    Matrix<float> result = cal.getSmoothed();

//    parallel<float>(test);


    // imshow("image",dst);
    // waitKey(0);
    // return 0;
}

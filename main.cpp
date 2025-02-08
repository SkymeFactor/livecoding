#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <fstream>

namespace my 
{
    class String
    {
    private:
        size_t size_ {};
        size_t capacity_ {};
        char* arr_ {nullptr};
    public:
        String()
        {
            size_ = 1;
            capacity_ = 0;
            arr_ = new char[size_];
        }

        String(const char* str): size_(std::strlen(str))
        {
            capacity_ = size_;
            arr_ = new char[size_]{};
            std::copy(str, str + size_, arr_);
        }

        String(const size_t& n): size_(n), capacity_(n)
        {
            arr_ = new char[size_]{};
        }

        String(const String& other): size_(other.size_), capacity_(other.capacity_)
        {
            arr_ = new char[size_]{};
            std::copy(other.arr_, other.arr_ + other.size_, arr_);
        }

        String& operator=(const String& other)
        {
            if (&other == this)
            {
                return *this;
            }

            delete[] arr_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            arr_ = new char[size_]{};
            std::copy(other.arr_, other.arr_ + other.size_, arr_);

            return *this;
        }

        ~String()
        {
            delete[] arr_;
        }

        void reallocate()
        {
            char* temp = arr_;
            arr_ = new char[2 * size_ + 1];
            std::copy(temp, temp + size_, arr_);
            size_ = size_ * 2 + 1;
        }

        void push_back(const char& x)
        {
            if (capacity_ + 1 >= size_)
            {
                reallocate();
            }

            arr_[capacity_] = x;
            ++capacity_;
        }

        size_t size()
        {
            return capacity_;
        }

        bool empty()
        {
            return capacity_ <= 0;
        }

        char pop_back()
        {
            --capacity_;

            return arr_[capacity_];
        }

        char& operator[](const size_t& index)
        {
            return arr_[index];
        }

        String substr(const size_t& left, const size_t& right)
        {
            String result(right - left + 1);
            for (size_t i = left; i <= right; ++i)
            {
                result[i - left] = arr_[i];
            }

            return result;
        }

        bool find(const char* str)
        {
            size_t size = std::strlen(str);
            if (size > this->size())
            {
                return false;
            }

            for (int i = 0; i < capacity_; ++i)
            {
                String temp = *this->substr(i, i + size);
                if (std::strcmp(temp, str) == 0)
                {
                    return true;
                }
            }
            
            return false;
        }

        operator char*()
        {
            return arr_;
        }

        friend std::ostream& operator<<(std::ostream& os, const String& obj)
        {
            os << obj.arr_;

            return os;
        }
    };
    
    template <typename T>
    class Vector
    {
        private:
        size_t size_ {};
        size_t capacity_ {};
        T* arr_ {nullptr};
    public:
        Vector()
        {
            size_ = 1;
            capacity_ = 0;
            arr_ = new T[size_];
        }

        Vector(const size_t& n): size_(n), capacity_(n)
        {
            arr_ = new T[size_]{};
        }

        Vector(const Vector& other): size_(other.size_), capacity_(other.capacity_)
        {
            arr_ = new T[size_]{};
            std::copy(other.arr_, other.arr_ + other.size_, arr_);
        }

        Vector& operator=(const Vector& other)
        {
            if (&other == this)
            {
                return *this;
            }

            delete[] arr_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            arr_ = new Vector[size_]{};
            std::copy(other.arr_, other.arr_ + other.size_, arr_);

            return *this;
        }

        ~Vector()
        {
            delete[] arr_;
        }

        void reallocate()
        {
            T* temp = arr_;
            arr_ = new T[2 * size_ + 1];
            std::copy(temp, temp + size_, arr_);
            size_ = size_ * 2 + 1;
        }

        void push_back(const T& x)
        {
            if (capacity_ + 1 >= size_)
            {
                reallocate();
            }

            arr_[capacity_] = x;
            ++capacity_;
        }

        size_t size()
        {
            return capacity_;
        }

        bool empty()
        {
            return capacity_ <= 0;
        }

        T pop_back()
        {
            --capacity_;

            return arr_[capacity_];
        }

        T& operator[](const size_t& index)
        {
            return arr_[index];
        }
    };

    class Parser
    {
    private:
        const char* input_filename_ {};
        const char* filter_filename_ {};
        const char* input_arg_ = "--input";
        const char* filter_arg_ = "--filterdb";
    public:
        bool parse(int size, char** line)
        {
            for (size_t i = 0; i < size; ++i)
            {
                if (std::strcmp(line[i], input_arg_) == 0 && i + 1 < size)
                {
                    input_filename_ = line[i + 1];
                }

                if (std::strcmp(line[i], filter_arg_) == 0 && i + 1 < size)
                {
                    filter_filename_ = line[i + 1];
                }
            }

            if (input_filename_ && filter_filename_)
            {
                return true;
            }

            return false;
        }

        const char* GetInputFilename()
        {
            return input_filename_;
        }

        const char* GetFilterFilename()
        {
            return filter_filename_;
        }
    };
} // namespace my

void PrintErrorAndExit()
{
    std::cerr << "Error reading command line arguments!" << '\n';
    std::exit(EXIT_FAILURE);
}


int main(int argc, char** argv)
{
    my::Parser parser;
    if (!parser.parse(argc, argv))
    {
        PrintErrorAndExit();
    }

    std::fstream ffile(parser.GetFilterFilename());
    if (!ffile.is_open())
    {
        std::cerr << "Failed to open filterdb file!" << '\n';
        std::exit(EXIT_FAILURE);
    }

    const size_t kBufferSize = 1024;
    char* fbuffer = new char[kBufferSize];
    my::Vector<my::String> filter;
    while (ffile.getline(fbuffer, kBufferSize))
    {
        filter.push_back(my::String(fbuffer));
    }



    std::fstream ifile(parser.GetInputFilename());
    if (!ifile.is_open())
    {
        std::cerr << "Failed to open input file!" << '\n';
        std::exit(EXIT_FAILURE);
    }

    char* ibuffer = new char[kBufferSize];
    my::Vector<my::String> words;
    while (ifile.getline(ibuffer, kBufferSize))
    {
        int i = 0;
        while (i < std::strlen(ibuffer))
        {
            int left = i;
            while (i < std::strlen(ibuffer) && ibuffer[i] != ' ' && ibuffer[i] != '\n')
            {
                ++i;
            }

            if (i - left > 1)
            {
                my::String str(ibuffer);
                words.push_back(str.substr(left,i - 1));
            }
            ++i;
        }
    }

    /*
        не успел дописать исключение лишних слов
        для этого была написана вспомогательная функция find
    */

    return 0;
}

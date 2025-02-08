#include <cstddef>
#include <iosfwd>
#include <iostream>
#include <cstring>

#include <fstream>

namespace __detail {

template<typename DataT>
struct ListNode {
  DataT data_;

  ListNode* next_node_ptr_;
  ListNode* prev_node_ptr_;
};

template<size_t size>
class FixString {
 public:
  FixString() {
    data[size] = '\0';
  }

  FixString& operator=(const FixString& value) {
    size_t offset = 0;
    while(offset != size + 1) {
      data[offset] = value.data[offset];
      ++offset;
    }
  };

  FixString(const FixString& value) {
    size_t offset = 0;
    while(offset != size + 1) {
      data[offset] = value.data[offset];
      ++offset;
    }
  }

  void ClearChars() {
    size_t offset = 0;
    while(offset != size + 1) {
      data[offset] = 0;
      ++offset;
    }
  }
  char* GetData() { return data; };
  const char* GetData() const { return data; };

  char& operator[](size_t index) { return data[index]; };
  const char& operator[](size_t index) const { return data[index]; };

 private:
  char data[size + 1];
};


template<size_t size1, size_t size2>
bool IsEqFixString(const FixString<size1>& string1, const FixString<size2>& string2) {
  if constexpr (size1 != size2) {
    return false;
  } else {
    auto register_killer = [](char ch) -> char {
      if (ch >= 'A' && ch <= 'Z')
        return ch - ('A' - 'a');
      return ch;
    };

    size_t index = 0;
    
    char cmp_ch1;
    char cmp_ch2;
    while (index != size1) {
      cmp_ch1 = register_killer(string1[index]);
      cmp_ch2 =  register_killer(string2[index]);
      if (cmp_ch1 != cmp_ch2)
        return false;
    }
    return true;
  }
}

} // namespace __detail

template<typename DataT>
class List {
 private:
  using Node = __detail::ListNode<DataT>;
 public:
    template<typename DataFwT>
    void AddBack(DataFwT&& data) {
      if (!end_ptr_) {
        beg_ptr_ = end_ptr_ = new Node{std::forward<DataFwT>(data), nullptr, nullptr};
      } else {
        end_ptr_->next_node_ptr_ = new Node{std::forward<DataFwT>(data), nullptr, end_ptr_};
        end_ptr_ = end_ptr_ ->next_node_ptr_;
      }
      ++size_;
    }

    template<typename DataFwT>
    void AddFront(DataFwT&& data) {
      if (!beg_ptr_) {
        beg_ptr_ = end_ptr_ = new Node{std::forward<DataFwT>(data), nullptr, nullptr};
      } else {
        beg_ptr_->prev_node_ptr_ = new Node{std::forward<DataFwT>(data), beg_ptr_, nullptr};
        beg_ptr_ = beg_ptr_->prev_node_ptr_;
      }
      ++size_;
    }

  Node* GetNodePtr(size_t index) const {
    Node* ret_node = beg_ptr_;
    while(index--) {
      ret_node = ret_node->next_node_ptr_;
    }
    return ret_node;
  };

  Node* GetBegNodePtr() const {
    return beg_ptr_;
  };

  Node* GetEndNodePtr() const {
    return end_ptr_;
  };

  DataT& operator[](size_t index) {
    return GetNodePtr(index)->data_;
  };

  const DataT& operator[](size_t index) const {
    return GetNodePtr(index)->data_;
  };

 public:
  void Clear() {
    Node* tmp_ptr;

    while (beg_ptr_ != nullptr) {
      tmp_ptr = beg_ptr_;
      beg_ptr_ = beg_ptr_->next_node_ptr_;
      delete tmp_ptr;
    }
  }

  size_t Size() const { return size_; };

  ~List() { Clear(); };

 private:
  Node* beg_ptr_;
  Node* end_ptr_;
  size_t size_;
};

class FileDeviceBase {
 public:
  FileDeviceBase(char* filename) {
    istream_.open(filename);
    if (!istream_.is_open()) {
      std::cout << "filter file can not open" << std::endl;
      state_ = false;
    } else {
      state_ = true;
    }
  };

  bool IsFails() const { return state_; };

 protected:
  std::ifstream istream_;
  bool state_ = false;
};

class FilterDevice : public FileDeviceBase {
 private:
  static constexpr size_t kBufferSize = 128;
  using BufferT = __detail::FixString<kBufferSize>;

 public:
  FilterDevice(char* filename) : FileDeviceBase(filename) {
    FilterParse();
  };

  bool IsPathernBuff(const BufferT& buffer) const {
    bool is_pathern = false;

    size_t index = 0;
    while (index != filter_list.Size()) {
      if (IsEqFixString(filter_list[index], buffer))
        return true;
      ++index;
    }
    return false;
  };

 private:
  void FilterParse() {
    auto is_separator = [](char ch) {
      return ch == '\t' || ch == '\n' || ch == '\r' || '\0';
    };

    char ch_buff = '3';
    BufferT w_buff;
    size_t w_buff_index = 0;

    while (!istream_.eof() && !istream_.fail()) {
      istream_.get(ch_buff);

      if (!is_separator(ch_buff)) {
        w_buff[w_buff_index++] = ch_buff;
      }

      if (is_separator(ch_buff) && !w_buff[0]) {
        filter_list.AddBack(w_buff);
        w_buff.ClearChars();

        w_buff[w_buff_index++] = ch_buff;
      }
    }
  };

 private:
  List<BufferT> filter_list;
};

class AnalizerDevice : public FileDeviceBase {
 public:
  AnalizerDevice(char* ifilename, char* ofilename) : FileDeviceBase(ifilename), ofilename_(ofilename) {

  };

  void Analizing(const FilterDevice& filter) {
    std::streampos pos = istream_.tellg();
    __detail::FixString<128> read_buff;

    std::ofstream ostream;
    ostream.open(ofilename_);
    if (!ostream.is_open()) {
      std::cout << "can't open in file" << std::endl;
      state_ = false;
      return;
    }

    while (!istream_.eof() && !istream_.fail()) {


      istream_.read(read_buff.GetData(), 128);
        if (!filter.IsPathernBuff(read_buff)) {
          
        }

    }
  };
 private:
  char* ofilename_;
};


int main(int, char**) {
  return 0;
};
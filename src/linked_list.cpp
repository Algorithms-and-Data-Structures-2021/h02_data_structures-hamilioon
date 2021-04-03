#include "linked_list.hpp"

#include <cassert>    // assert
#include <stdexcept>  // out_of_range

#include "private/internal.hpp"  // это не тот приват, о котором вы могли подумать

namespace itis {

void LinkedList::Add(Element e) {
  // Tip 1: создайте узел в куче со переданным значением
  // Tip 2: есть 2 случая - список пустой и непустой
  // Tip 3: не забудьте обновить поля head и tail
  // напишите свой код здесь ...
  if (tail() == Element::UNINITIALIZED){
     head_ = new Node(e,nullptr);
     tail_ = head_;

  }
  else{
      Node *new_node = new Node(e, nullptr);
      tail_->next = new_node;
      tail_ = new_node;

  }
  size_++;


}

void LinkedList::Insert(int index, Element e) {
  internal::check_out_of_range(index, 0, size_ + 1);

  // Tip 1: вставка элементов на позицию size эквивалентно операции добавления в конец
  // Tip 2: рассмотрите несколько случаев:
  //        (1) список пустой,
  //        (2) добавляем в начало списка,
  //        (3) добавляем в конец списка
  //        (4) все остальное

  // напишите свой код здесь ...
  if (tail() == Element::UNINITIALIZED && index == size_ - 1){
      Add(e);
  }
  else if ( index == 0){
      if (size_ == 1){
          Node *pointer = new Node(e, head_);
          tail_ = head_;
          head_ = pointer;
          size_++;
      }
      else {
          Node *pointer = new Node(e, head_);
          head_ = pointer;
          size_++;
      }
  }
   else if ( index == size_){
      Node *pointer = new Node(e, nullptr);
      tail_->next = pointer;
      tail_ = pointer;
      size_++;
  }

  else {
      Node *desired_pos = find_node(index);
      Node *before_pos = find_node(index - 1);
      Node *new_node = new Node(e,desired_pos);
      before_pos->next = new_node;
      size_++;
  }

}

void LinkedList::Set(int index, Element e) {
  internal::check_out_of_range(index, 0, size_);
  // Tip 1: используйте функцию find_node(index)
  // напишите свой код здесь ...
  find_node(index)->data = e;
}

Element LinkedList::Remove(int index) {
  internal::check_out_of_range(index, 0, size_);
  // Tip 1: рассмотрите случай, когда удаляется элемент в начале списка
  // Tip 2: используйте функцию find_node(index)
  // напишите свой код здесь ...
  if (index == 0){
      auto result = head_->data;
      head_ = head_->next;
      size_ --;
      return result;
  }
  else if ( index == size_ - 1){
      auto result = find_node(index)->data;
      Node *pointer = find_node(index - 1);
      pointer->next = nullptr;
      tail_ = pointer;
      size_ --;
      return result;
  }
  else{
      auto result = find_node(index)->data;
      Node *before_index = find_node(index - 1);
      Node *after_index = find_node(index + 1);
      before_index->next = after_index;
      size_ --;
      return result;

  }
  return {};
}

    void LinkedList::Clear()
    {
        // Tip 1: люди в черном (MIB) пришли стереть вам память

        Node *current_node = head_;
        Node *delete_node;
        size_ = 0;
        head_ = nullptr;
        tail_ = nullptr;

        while (current_node != nullptr)
        {
            delete_node = current_node;
            current_node = current_node -> next;
            delete delete_node;
        }
    }

Element LinkedList::Get(int index) const {
  internal::check_out_of_range(index, 0, size_);
  // напишите свой код здесь ...
  return find_node(index)->data;
}

int LinkedList::IndexOf(Element e) const {
  // напишите свой код здесь ...
    if (size_ == 0){
        return kNotFoundElementIndex;
    }
    auto *pointer = head_;
    if (pointer->data == e){
        return 0;
    }
  for ( int i = 0; i < size_ - 1; i++){
      pointer = pointer->next;
      if (pointer->data == e){
          return i + 1;
      }
  }
  return kNotFoundElementIndex;
}

Node *LinkedList::find_node(int index) const {
  assert(index >= 0 && index < size_);
  // Tip 1: можете сразу обработать случаи поиска начала и конца списка
  // напишите свой код здесь ...


  if(index == size_ - 1){
      return tail_;
  }
  if(index == 0){
      return head_;
    }

  auto *pointer = head_;
  for(int i = 0; i < index; i++){
      pointer = pointer->next;
  }
  return pointer;
}

// РЕАЛИЗОВАНО

LinkedList::~LinkedList() {
  Clear();
}

bool LinkedList::Contains(Element e) const {
  // если индекс не найден, значит и элемента нет
  return IndexOf(e) == kNotFoundElementIndex;
}

int LinkedList::GetSize() const {
  return size_;
}

bool LinkedList::IsEmpty() const {
  return size_ == 0;
}

Element LinkedList::tail() const {
  // вместо выброса ошибки в случае nullptr, римским парламентов было решено возвращать "специальное" значение
  return tail_ ? tail_->data : Element::UNINITIALIZED;
}

Element LinkedList::head() const {
  return head_ ? head_->data : Element::UNINITIALIZED;
}

// === RESTRICTED AREA: необходимо для тестирования ===

LinkedList::LinkedList(const std::vector<Element> &elements) {
  assert(!elements.empty());

  size_ = elements.size();
  head_ = new Node(elements[0], nullptr);

  auto current_node = head_;

  for (int index = 1; index < static_cast<int>(elements.size()); index++) {
    current_node->next = new Node(elements[index], nullptr);
    current_node = current_node->next;
  }
  tail_ = current_node;
}

std::ostream &operator<<(std::ostream &os, const LinkedList &list) {
  if (list.head_ != nullptr && list.tail_ != nullptr) {
    os << "{ ";
    for (auto current_node = list.head_; current_node != list.tail_; current_node = current_node->next) {
      os << internal::elem_to_str(current_node->data) << ", ";
    }
    os << internal::elem_to_str(list.tail_->data) << " }";
  } else {
    os << "{ nullptr }";
  }
  return os;
}

bool operator==(const LinkedList &list, const std::vector<Element> &elements) {
  if (list.size_ != static_cast<int>(elements.size())) return false;
  Node *current_node = list.head_;

  for (const auto e : elements) {
    if (current_node == nullptr) return false;
    if (current_node->data != e) return false;
    current_node = current_node->next;
  }
  return true;
}

}  // namespace itis
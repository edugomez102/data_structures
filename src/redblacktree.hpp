#pragma once 

#include "pair.hpp"
#include <stdio.h>

// TODO: clean up
// TODO: delete node

namespace edgs {

  /**
   * struct RBtree - Simple red-black-tree implementation.
   *
   * @tparam T data type to be stored. 
   *         It needs to have operator< overloaded
   * @tparam K KeyOfValue, key type of the tree(in case T is a pair)
   */
  template <typename T, typename K = T>
  struct RBtree{

    enum color_t { RED = false, BLACK = true };

    struct Node{
      T data_{};
      Node* parent_{};
      Node* left_{};
      Node* right_{};
      color_t color_{ RED };

      void recolor() {
        if(color_ == RED) color_ = BLACK;
        else color_ = RED;
      }
      Node* grandpa() { return parent_->parent_;}

      // luncle
      // runcle
    };

    ~RBtree() {
      clear();
    }

    void clear() {
      if (size_ > 0)
      {
        clear_impl(root_);
        size_ = 0;
      }
    }

    pair<Node*, bool> insert(const T& v){
      Node* n = new Node{v};
      insert_impl(root_, n);
      insert_fixup(n);

      if(n->parent_ == nullptr && n != root_)
      {
        delete n; // duplicate
        return pair<Node*, bool>(nullptr, false);
      }
      else {
        ++size_;
        return pair(n, true);
      }
    }

    Node* find(const K& key){
      Node* n = root_;
      while (n != nullptr && n->data_ != key) {
        if (n->data_ > key)
          n = n->left_;
        else
          n = n->right_;
      }
      return n;
    }

    Node* upper_bound(const K& key){
      return bound_impl(key, UPPER);
    }

    Node* lower_bound(const K& key){
      return bound_impl(key, LOWER);
    }

    Node* min(){
      Node* n = root_;
      while(n && n->left_ != nullptr) n = n->left_;
      return n;
    }

    Node* max(){
      Node* n = root_;
      while(n && n->right_ != nullptr) n = n->right_;
      return n;
    }

    size_t size() { return size_; }

    static Node* rb_tree_increment(Node* n){
      if(!n) return n;
      if(n->right_ != nullptr)
      {
        n = n->right_;
        while(n->left_ != nullptr)
          n = n->left_;
      }
      else 
      {
        Node* tmp = n->parent_;
        while(tmp && n == tmp->right_)
        {
          n = tmp;
          tmp = tmp->parent_;
        }
        if(n->right_ != tmp)
          n = tmp;
        else n = nullptr;
      }
      return n;
    }

    static Node* rb_tree_decrement(Node* n){
      if(n->color_ == RED && n->parent_->parent_ == n){
        n = n->right_;
      }
      else if( n->left_ != nullptr){
        Node* tmp = n->left_;
        while(tmp->right_ != nullptr)
          tmp = tmp->right_;
        n = tmp;
      }
      else {
        Node* tmp = n->parent_;
        while( n == tmp->left_)
        {
          n = tmp;
          tmp = tmp->parent_;
        }
        n = tmp;
      }
      return n;
    }

  private:

    void clear_impl(Node* n) {
      if( n != nullptr) {
        clear_impl(n->right_);
        clear_impl(n->left_);
        delete n;
      }
    };

    void insert_impl(Node* root, Node* n){
      if(root == nullptr)
        root_ = n;
      else if(n->data_ < root->data_) {
        if(root->left_ == nullptr){
          root->left_ = n;
          n->parent_ = root;
        }
        else insert_impl(root->left_, n);
      }
      else if(n->data_ > root->data_) {
        if(root->right_ == nullptr) {
          root->right_ = n;
          n->parent_ = root;
        }
        else insert_impl(root->right_, n);
      }
    }

    void insert_fixup(Node* n) {
      while(n->parent_ && n->parent_->color_ == color_t::RED){
        if(n->parent_ == n->parent_->parent_->left_) {
          Node* uncle = n->parent_->parent_->right_;
          if(uncle && uncle->color_ == color_t::RED) {
            n->parent_->recolor();
            n->parent_->parent_->recolor();
            uncle->recolor();
            n = n->parent_->parent_; // loop
          }
          else{
            if(n == n->parent_->right_) {
              n = n->parent_;
              leftRotate(n);
            }
            n->parent_->recolor();
            n->parent_->parent_->recolor();
            rightRotate(n->parent_->parent_);
          }
        }
        // same opposite
        else if(n->parent_ == n->parent_->parent_->right_)
        {
          Node* uncle = n->parent_->parent_->left_;
          if(uncle && uncle->color_ == color_t::RED)
          {
            n->parent_->recolor();
            n->parent_->parent_->recolor();
            uncle->recolor();
            n = n->parent_->parent_; // loop
          }
          else {
            if(n == n->parent_->left_) {
              n = n->parent_;
              rightRotate(n);
            }
            n->parent_->recolor();
            n->parent_->parent_->recolor();
            leftRotate(n->parent_->parent_);
          }
        }
      }
      root_->color_ = color_t::BLACK;
    }

    void leftRotate(Node* n){
      Node* tmp = n->right_;
      n->right_ = tmp->left_;
      if(tmp->left_ != nullptr)
        tmp->left_->parent_ = n;

      tmp->parent_ = n->parent_; 
      if(n->parent_ == nullptr)
        root_ = tmp;
      else if(n == n->parent_->left_)
        n->parent_->left_ =  tmp;
      else if(n == n->parent_->right_)
        n->parent_->right_ =  tmp;

      tmp->left_ = n;
      n->parent_ = tmp;
    }

    void rightRotate(Node* n){
      Node* tmp = n->left_;
      n->left_ = tmp->right_;
      if(tmp->right_ != nullptr)
        tmp->right_->parent_ = n;

      tmp->parent_ = n->parent_; 
      if(n->parent_ == nullptr)
        root_ = tmp;
      else if(n == n->parent_->right_)
        n->parent_->right_ =  tmp;
      else if(n == n->parent_->left_)
        n->parent_->left_ =  tmp;

      tmp->right_ = n;
      n->parent_ = tmp;
    }

    Node* root_{nullptr};
    size_t size_{0};

    enum bound_t { UPPER = false, LOWER = true };
    Node* bound_impl(const K& key, bound_t bt ){
      Node* current = root_;
      Node* result = nullptr;

      while (current != nullptr) {
        if ( bt ? current->data_ >= key :
                  current->data_ > key) {
          result = current;
          current = current->left_;
        } else {
          current = current->right_;
        }
      }
      return result;
    }

  };

}

/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
    typedef pair<const Key, T> value_type;

private:
    struct node {
        node* left;
        node* right;
        node* parent;
        int height;
        value_type value;
        node(node* p, const value_type& v):
                left(nullptr), right(nullptr), parent(p), height(1), value(v) {}
        node(node* l, node* r, int h, const value_type& v):
            left(l), right(r), parent(nullptr), height(h), value(v) {};

        void update_height() {
            int lh = left ? left->height : 0;
            int rh = right ? right->height : 0;
            height = lh > rh ? lh : rh;
            ++height;
        }

        bool is_lchild() {
            return parent && parent->left == this;
        }

        bool is_rchild() {
            return parent && parent->right == this;
        }

        int recalc_height(bool &balance) {
            int lh = left ? left->height : 0;
            int rh = right ? right->height : 0;

            balance = lh <= rh + 1 && rh <= lh + 1;

            int ret =  lh > rh ? lh : rh;
            return ret + 1;
        }

        node* taller_child() const {
            int lh = left ? left->height : 0;
            int rh = right ? right->height : 0;
            return lh > rh ? left : right;
        }


        node* &rightmost_lchild() {
            if (!left) {
                throw;
            }

            node** ret = &left;
            while ((*ret)->right) {
                ret = &(*ret)->right;
            }

            return *ret;
        }

        node* &leftmost_rchild() {
            if (!right) {
                throw;
            }

            node** ret = &right;
            while ((*ret)->left) {
                ret = &(*ret)->left;
            }

            return *ret;
        };

    };

    size_t map_size;
    node* root;
    Compare comp;

public:
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
        friend class map;
        friend class const_iterator;
		const map* this_map;
        node* this_node;
	public:
		iterator():this_map(nullptr), this_node(nullptr) {}
        iterator(const map* m, node* n): this_map(m), this_node(n) {}
		iterator(const iterator &other) = default;

		iterator operator++(int) {
            if (!this_node) {
                throw invalid_iterator();
            }

            iterator ret {*this};

            if (this_node->right) {
                this_node = this_node->leftmost_rchild();
            } else {
                while (this_node->is_rchild()) {
                    this_node = this_node->parent;
                }
                this_node = this_node->parent;
            }

            return ret;
        }
		iterator & operator++() {
            if (!this_node) {
                throw invalid_iterator();
            }

            if (this_node->right) {
                this_node = this_node->leftmost_rchild();
            } else {
                while (this_node->is_rchild()) {
                    this_node = this_node->parent;
                }
                this_node = this_node->parent;
            }

            return *this;
        }

		iterator operator--(int) {
            if (*this == this_map->begin() ) {
                throw invalid_iterator();
            }

            iterator ret {*this};

            if (!this_node) {
                node* this_node = this_map->root;

                if (!this_node) {
                    return ret;
                }

                while (this_node->right) {
                    this_node = this_node->right;
                }
            } else {
                if (this_node->left) {
                    this_node = this_node->rightmost_lchild();
                } else {
                    while (this_node->is_lchild()) {
                        this_node = this_node->parent;
                    }
                    this_node = this_node->parent;
                }
            }

            return ret;
        }
		iterator & operator--() {
            if (*this == this_map->begin() ) {
                throw invalid_iterator();
            }

            if (!this_node) {
                this_node = this_map->root;

                if (!this_node) {
                    return *this;
                }

                while (this_node->right) {
                    this_node = this_node->right;
                }
            } else {
                if (this_node->left) {
                    this_node = this_node->rightmost_lchild();
                } else {
                    while (this_node->is_lchild()) {
                        this_node = this_node->parent;
                    }
                    this_node = this_node->parent;
                }
            }

            return *this;
        }

		value_type & operator*() const {
            return this_node->value;
        }
        value_type* operator->() const noexcept {
            return &this_node->value;
        }

		bool operator==(const iterator &rhs) const {
            return this_map == rhs.this_map && this_node == rhs.this_node;
        }
		bool operator==(const const_iterator &rhs) const {
            return rhs == *this;
        }

		bool operator!=(const iterator &rhs) const {
            return this_map != rhs.this_map || this_node != rhs.this_node;
        }
		bool operator!=(const const_iterator &rhs) const {
            return rhs != *this;
        }

	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
    private:
        const map* this_map;
        node* this_node;
    public:
        const_iterator() = default;
        const_iterator(const map* m, node* n): this_map(m), this_node(n) {}
        const_iterator(const const_iterator &other) = default;
        const_iterator(const iterator &other) {
            this_node = other.this_node;
            this_map = other.this_map;
        }

        const_iterator operator++(int) {
            if (!this_node) {
                throw invalid_iterator();
            }

            const_iterator ret {*this};

            if (this_node->right) {
                this_node = this_node->leftmost_rchild();
            } else {
                while (this_node->is_rchild()) {
                    this_node = this_node->parent;
                }
                this_node = this_node->parent;
            }

            return ret;
        }
        const_iterator& operator++() {
            if (!this_node) {
                throw invalid_iterator();
            }

            if (this_node->right) {
                this_node = this_node->leftmost_rchild();
            } else {
                while (this_node->is_rchild()) {
                    this_node = this_node->parent;
                }
                this_node = this_node->parent;
            }
            return *this;
        }

        const_iterator operator--(int) {
            if (*this == this_map->cbegin() ) {
                throw invalid_iterator();
            }

            const_iterator ret {*this};

            if (!this_node) {
                this_node = this_map->root;

                if (!this_node) {
                    return ret;
                }

                while (this_node->right) {
                    this_node = this_node->right;
                }
            } else {
                if (this_node->left) {
                    this_node = this_node->rightmost_lchild();
                } else {
                    while (this_node->is_lchild()) {
                        this_node = this_node->parent;
                    }
                    this_node = this_node->parent;
                }
            }

            return ret;
        }
        const_iterator& operator--() {

            if (*this == this_map->cbegin()) {
                throw invalid_iterator();
            }

            if (!this_node) {
                this_node = this_map->root;

                if (!this_node) {
                    return *this;
                }

                while (this_node->right) {
                    this_node = this_node->right;
                }
            } else {
                if (this_node->left) {
                    this_node = this_node->rightmost_lchild();
                } else {
                    while (this_node->is_lchild()) {
                        this_node = this_node->parent;
                    }
                    this_node = this_node->parent;
                }
            }

            return *this;
        }

        bool operator==(const iterator &rhs) const {
            return this_map == rhs.this_map && this_node == rhs.this_node;
        }
        bool operator==(const const_iterator &rhs) const {
            return this_map == rhs.this_map && this_node == rhs.this_node;
        }

        bool operator!=(const iterator &rhs) const {
            return this_map != rhs.this_map || this_node != rhs.this_node;
        }
        bool operator!=(const const_iterator &rhs) const {
            return this_map != rhs.this_map || this_node != rhs.this_node;
        }

        const value_type* operator->() {
            return &this_node->value;
        }
        const value_type& operator*() {
            return this_node->value;
        }
	};

	map():map_size(0), root(nullptr),  comp() {}
	map(const map &other) {
        root = clone_tree(other.root);
        comp = other.comp;
        map_size = other.map_size;
    }

	map & operator=(const map &other) {
        if (this == &other) {
            return *this;
        }

        clear();

        root = clone_tree(other.root);
        comp = other.comp;
        map_size = other.map_size;

        return *this;
    }

	~map() {
        clear();
    }

private:
    node* insert_helper(const value_type value) {
        node* hot;
        node* &to_insert = search_node(value.first, hot);

        if (to_insert) {
            return to_insert;
        }

        ++map_size;
        auto ret = new node(hot, value);
        // the value of to_insert might be set to something else during the rebalance
        // however, the address of the new node will not change
        to_insert = ret;


        // adjust height and rebalance
        while (hot) {
            bool balance;
            int new_height = hot->recalc_height(balance);

            if (new_height == hot->height) {
                break;
            }

            hot->height = new_height;
            if (!balance) {
                node* tmp = hot->parent;
                node* &pip = pointer_in_parent(hot);
                pip = rotate(hot);
                pip->parent = tmp;
                break;
            }

            hot = hot->parent;
        }

        return ret;
    }

    node* search_node(const Key &key) const {
        node* pos = root;

        while(pos) {
            if (comp(key, pos->value.first)) {
                pos = pos->left;
            } else if (comp(pos->value.first, key)) {
                pos = pos->right;
            } else {
                break;
            }
        }

        return pos;
    }

    node* &search_node(const Key &key, node* &_hot) {
        _hot = nullptr;
        node** ret = &root;

        while (*ret) {
            node* this_node = *ret;
            const Key &this_key = this_node->value.first;
            if (comp(key, this_key)) {
                ret = &this_node->left;
                _hot = this_node;
            } else if(comp(this_key, key)) {
                ret = &this_node->right;
                _hot = this_node;
            } else {
                return *ret;
            }
        }

        return *ret;
    }

    node* clone_tree(const node* tree) {

        if (!tree) {
            return nullptr;
        }


        node* left = clone_tree(tree->left);
        node* right = clone_tree(tree->right);
        auto ret = new node(left, right, tree->height, tree->value);

        if (left) left->parent = ret;
        if (right) right->parent = ret;
        return ret;
    }

    void clear(node* tree) {
        if (tree) {
            clear(tree->left);
            clear(tree->right);
            delete tree;
        }
    }

    node* &pointer_in_parent(node* p) {
        if (p->is_lchild()) {
            return p->parent->left;
        } else if (p->is_rchild()) {
            return p->parent->right;
        } else {
            return root;
        }
    }

    node* rotate(node* p) {
        node* c = p->taller_child();
        node* gc = c->taller_child();
        node* ret;
        if (gc->is_lchild()) {
            if (c->is_lchild()) {
                ret = connect34(gc, c, p, gc->left, gc->right, c->right, p->right);
            } else {
                ret = connect34(p, gc, c, p->left, gc->left, gc->right, c->right);
            }
        } else {
            if (c->is_lchild()) {
                ret = connect34(c, gc, p, c->left, gc->left, gc->right, p->right);
            } else {
                ret = connect34(p, c, gc, p->left, c->left, gc->left, gc->right);
            }
        }

        return ret;
    }

    node* connect34(node* a, node* b, node* c, node* t1, node* t2, node* t3, node* t4) {
        a->left = t1;
        a->right = t2;
        if (t1) t1->parent = a;
        if (t2) t2->parent = a;
        a->update_height();

        c->left = t3;
        c->right = t4;
        if (t3) t3->parent = c;
        if (t4) t4->parent = c;
        c->update_height();

        b->left = a;
        b->right = c;
        a->parent = b;
        c->parent = b;
        b->update_height();

        return b;
    }

public:

	/**
	 * Returns a reference to the mapped value
	 * an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {

        node* res = search_node(key);

        if (!res) {
            throw index_out_of_bound();
        }

        return res->value.second;
    }
	const T & at(const Key &key) const {

        node* res = search_node(key);

        if (!res) {
            throw index_out_of_bound();
        }

        return res->value.second;
    }

	/**
	 * access specified element
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {

        node* res = search_node(key);

        if (!res) {
            res = insert_helper(value_type(key, T()));
        }

        return res->value.second;
    }
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
        node* res = search_node(key);

        if (!res) {
            throw index_out_of_bound();
        }

        return res->value.second;
    }

	iterator begin() const {
        if (!root) {
            return iterator(this, nullptr);
        }

        node* ret = root;
        while (ret->left) ret = ret->left;

        return iterator(this, ret);
    }
	const_iterator cbegin() const {
        if (!root) {
            return cend();
        }

        node* ret = root;
        while (ret->left) ret = ret->left;

        return const_iterator(this, ret);
    }

	iterator end() const{
        return iterator(this, nullptr);
    }
	const_iterator cend() const {
        return const_iterator(this, nullptr);
    }

	bool empty() const {
        return !map_size;
    }

	size_t size() const {
        return map_size;
    }

	void clear() {
        clear(root);
        map_size = 0;
        root = nullptr;
    }

	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(value_type value) {

        node* res = search_node(value.first);

        if (res) {
            return pair<iterator, bool>(iterator(this, res), false);
        } else {
            res = insert_helper(value);
            return pair<iterator, bool>(iterator(this, res), true);
        }
    }

	/**
	 * erase the element at pos.
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {

        node* lowest_height_change = nullptr;
        node* to_remove = pos.this_node;

        if (pos.this_map != this || !to_remove) {
            throw index_out_of_bound();
        }

        if (to_remove->left && to_remove->right) {
            // copy this one to the original position
            node* leftmost_rchild = to_remove->leftmost_rchild();
            auto new_node = new node(to_remove->left, to_remove->right,
                                      to_remove->height, leftmost_rchild->value);
            new_node->parent = to_remove->parent;
            if (to_remove->left) to_remove->left->parent = new_node;
            if (to_remove->right) to_remove->right->parent = new_node;

            pointer_in_parent(to_remove) = new_node;  // link from it's parent
            delete to_remove;

            // remove duplicated node
            to_remove = leftmost_rchild;
            pointer_in_parent(to_remove) = to_remove->right;

            if (to_remove->right)
                to_remove->right->parent = to_remove->parent;

            lowest_height_change = to_remove->parent;

            delete to_remove;
        } else {
            lowest_height_change = to_remove->parent;
            node* child = to_remove->left ? to_remove->left : to_remove->right;
            if (child) child->parent = to_remove->parent;

            node* &pip = pointer_in_parent(to_remove);
            pip = child;

            delete to_remove;
        }

        --map_size;
        node* hot = lowest_height_change;
        while(hot) {
            bool balance;
            hot->height = hot->recalc_height(balance);

            if (!balance) {
                node* tmp = hot->parent;
                node* &pip = pointer_in_parent(hot);
                pip = rotate(hot);
                pip->parent = tmp;
                hot = tmp;
            } else {
                hot = hot->parent;
            }
        }
    }
	/**
	 * Returns the number of elements with key, either 1 or 0
	 */
	size_t count(const Key &key) const {
        node* res = search_node(key);

        return res ? 1 : 0;
    }
	/**
	 * Finds an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
        return iterator(this, search_node(key));
    }
	const_iterator find(const Key &key) const {

        return const_iterator(this, search_node(key));
    }
};
}

#endif
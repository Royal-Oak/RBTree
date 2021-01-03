#pragma once
#include <iomanip>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
enum RBTColor { RED, BLACK };

template <class T>
class RBTNode {
public:
	RBTColor color;    // 颜色
	T key;            // 关键字(键值)
	int count;			//树的节点数
	RBTNode *left;    // 左孩子
	RBTNode *right;    // 右孩子
	RBTNode *parent; // 父结点

	RBTNode(T value, RBTColor c, RBTNode *p, RBTNode *l, RBTNode *r) :
		key(value), color(c), parent(), left(l), right(r) {
		count = 1;
	}
};

template <class T>
class RBTree {
private:
	RBTNode<T> *mRoot;    // 根结点

public:
	RBTree();
	~RBTree();

	// 前序遍历"红黑树"
	void preOrder();
	// 中序遍历"红黑树"
	void inOrder();
	// 后序遍历"红黑树"
	void postOrder();

	// (递归实现)查找"红黑树"中键值为key的节点
	RBTNode<T>* search(T key);
	// (非递归实现)查找"红黑树"中键值为key的节点
	RBTNode<T>* iterativeSearch(T key);

	// 查找最小结点：返回最小结点的键值。
	T minimum();
	// 查找最大结点：返回最大结点的键值。
	T maximum();

	// 找结点(x)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
	RBTNode<T>* successor(RBTNode<T> *x);
	// 找结点(x)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
	RBTNode<T>* predecessor(RBTNode<T> *x);

	// 将结点(key为节点键值)插入到红黑树中
	void insert(T key);

	// 删除结点(key为节点键值)
	void remove(T key);

	// 销毁红黑树
	void destroy();

	// 打印红黑树
	void print();

	//获取key在树中的按从小到大的顺序位置，
	pair<int, int> rank(T val);

	// 查找排位为 k 的元素(index)
	T &operator[](int index);


private:
	// 前序遍历"红黑树"
	void preOrder(RBTNode<T>* tree) const;
	// 中序遍历"红黑树"
	void inOrder(RBTNode<T>* tree) const;
	// 后序遍历"红黑树"
	void postOrder(RBTNode<T>* tree) const;

	// (递归实现)查找"红黑树x"中键值为key的节点
	RBTNode<T>* search(RBTNode<T>* x, T key) const;
	// (非递归实现)查找"红黑树x"中键值为key的节点
	RBTNode<T>* iterativeSearch(RBTNode<T>* x, T key) const;

	// 查找最小结点：返回tree为根结点的红黑树的最小结点。
	RBTNode<T>* minimum(RBTNode<T>* tree);
	// 查找最大结点：返回tree为根结点的红黑树的最大结点。
	RBTNode<T>* maximum(RBTNode<T>* tree);

	// 左旋
	void leftRotate(RBTNode<T>* &root, RBTNode<T>* x);
	// 右旋
	void rightRotate(RBTNode<T>* &root, RBTNode<T>* y);
	// 插入函数
	void insert(RBTNode<T>* &root, RBTNode<T>* node);
	// 插入修正函数
	void insertFixUp(RBTNode<T>* &root, RBTNode<T>* node);
	// 删除函数
	void remove(RBTNode<T>* &root, RBTNode<T> *node);
	// 删除修正函数
	void removeFixUp(RBTNode<T>* &root, RBTNode<T> *node, RBTNode<T> *parent);

	// 销毁红黑树
	void destroy(RBTNode<T>* &tree);

	// 打印红黑树
	void print(RBTNode<T>* tree, T key, int direction);

	//获取当前结点key的计数
	int getSelfcount(RBTNode<T> *node) const;

	//获取key在树中的按从小到大的顺序位置，
	pair<int, int> rank(RBTNode<T> *root, T val);

	// 查找排位为 k 的元素(index)
	pair<int, T> getK(RBTNode<T> *root, int index);

#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_count(r) ((r)->count)
#define rb_is_red(r)   ((r)->color==RED)
#define rb_is_black(r)  ((r)->color==BLACK)
#define rb_set_black(r)  do { (r)->color = BLACK; } while (0)
#define rb_set_red(r)  do { (r)->color = RED; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { (r)->color = (c); } while (0)
};
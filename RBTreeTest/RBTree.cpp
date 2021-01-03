#include "pch.h"
#include <iostream>
#include "RBTree.h"
//#include <iomanip>

using namespace std;

/*
 * ���캯��
 */
template <class T>
RBTree<T>::RBTree() :mRoot(NULL)
{
	mRoot = NULL;
}

/*
 * ��������
 */
template <class T>
RBTree<T>::~RBTree()
{
	destroy();
}

/*
 * ǰ�����"�����"
 */
template <class T>
void RBTree<T>::preOrder(RBTNode<T>* tree) const
{
	if (tree != NULL)
	{
		int c = getSelfcount(tree);
		for (; c > 0; c--)
			cout << tree->key << " ";
		preOrder(tree->left);
		preOrder(tree->right);
	}
}

template <class T>
void RBTree<T>::preOrder()
{
	preOrder(mRoot);
}

/*
 * �������"�����"
 */
template <class T>
void RBTree<T>::inOrder(RBTNode<T>* tree) const
{
	if (tree != NULL)
	{
		inOrder(tree->left);
		int c = getSelfcount(tree);
		for (; c > 0; c--)
			cout << tree->key << " ";
		inOrder(tree->right);
	}
}

template <class T>
void RBTree<T>::inOrder()
{
	inOrder(mRoot);
}

/*
 * �������"�����"
 */
template <class T>
void RBTree<T>::postOrder(RBTNode<T>* tree) const
{
	if (tree != NULL)
	{
		postOrder(tree->left);
		postOrder(tree->right);
		int c = getSelfcount(tree);
		for (; c > 0; c--)
			cout << tree->key << " ";
	}
}

template <class T>
void RBTree<T>::postOrder()
{
	postOrder(mRoot);
}

/*
 * (�ݹ�ʵ��)����"�����x"�м�ֵΪkey�Ľڵ�
 */
template <class T>
RBTNode<T>* RBTree<T>::search(RBTNode<T>* x, T key) const
{
	if (x == NULL || x->key == key)
		return x;

	if (key < x->key)
		return search(x->left, key);
	else
		return search(x->right, key);
}

template <class T>
RBTNode<T>* RBTree<T>::search(T key)
{
	search(mRoot, key);
}

/*
 * (�ǵݹ�ʵ��)����"�����x"�м�ֵΪkey�Ľڵ�
 */
template <class T>
RBTNode<T>* RBTree<T>::iterativeSearch(RBTNode<T>* x, T key) const
{
	while ((x != NULL) && (x->key != key))
	{
		if (key < x->key)
			x = x->left;
		else
			x = x->right;
	}

	return x;
}

template <class T>
RBTNode<T>* RBTree<T>::iterativeSearch(T key)
{
	iterativeSearch(mRoot, key);
}

/*
 * ������С��㣺����treeΪ�����ĺ��������С��㡣
 */
template <class T>
RBTNode<T>* RBTree<T>::minimum(RBTNode<T>* tree)
{
	if (tree == NULL)
		return NULL;

	while (tree->left != NULL)
		tree = tree->left;
	return tree;
}

template <class T>
T RBTree<T>::minimum()
{
	RBTNode<T> *p = minimum(mRoot);
	if (p != NULL)
		return p->key;

	return (T)NULL;
}

/*
 * ��������㣺����treeΪ�����ĺ����������㡣
 */
template <class T>
RBTNode<T>* RBTree<T>::maximum(RBTNode<T>* tree)
{
	if (tree == NULL)
		return NULL;

	while (tree->right != NULL)
		tree = tree->right;
	return tree;
}

template <class T>
T RBTree<T>::maximum()
{
	RBTNode<T> *p = maximum(mRoot);
	if (p != NULL)
		return p->key;

	return (T)NULL;
}

/*
 * �ҽ��(x)�ĺ�̽�㡣��������"�����������ֵ���ڸý��"��"��С���"��
 */
template <class T>
RBTNode<T>* RBTree<T>::successor(RBTNode<T> *x)
{
	// ���x�����Һ��ӣ���"x�ĺ�̽��"Ϊ "�����Һ���Ϊ������������С���"��
	if (x->right != NULL)
		return minimum(x->right);

	// ���xû���Һ��ӡ���x���������ֿ��ܣ�
	// (01) x��"һ������"����"x�ĺ�̽��"Ϊ "���ĸ����"��
	// (02) x��"һ���Һ���"�������"x����͵ĸ���㣬���Ҹø����Ҫ��������"���ҵ������"��͵ĸ����"����"x�ĺ�̽��"��
	RBTNode<T>* y = x->parent;
	while ((y != NULL) && (x == y->right))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

/*
 * �ҽ��(x)��ǰ����㡣��������"�����������ֵС�ڸý��"��"�����"��
 */
template <class T>
RBTNode<T>* RBTree<T>::predecessor(RBTNode<T> *x)
{
	// ���x�������ӣ���"x��ǰ�����"Ϊ "��������Ϊ���������������"��
	if (x->left != NULL)
		return maximum(x->left);

	// ���xû�����ӡ���x���������ֿ��ܣ�
	// (01) x��"һ���Һ���"����"x��ǰ�����"Ϊ "���ĸ����"��
	// (01) x��"һ������"�������"x����͵ĸ���㣬���Ҹø����Ҫ�����Һ���"���ҵ������"��͵ĸ����"����"x��ǰ�����"��
	RBTNode<T>* y = x->parent;
	while ((y != NULL) && (x == y->left))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

/*
 * �Ժ�����Ľڵ�(x)��������ת
 */
template <class T>
void RBTree<T>::leftRotate(RBTNode<T>* &root, RBTNode<T>* x)
{
	// ����x���Һ���Ϊy
	RBTNode<T> *y = x->right;

	// �� ��y�����ӡ� ��Ϊ ��x���Һ��ӡ���
	// ���y�����ӷǿգ��� ��x�� ��Ϊ ��y�����ӵĸ��ס�
	x->right = y->left;
	if (y->left != NULL) {
		y->left->parent = x;
		//�ı�ptr��count��
		x->count = x->count - y->count + y->left->count;
	}
	else
		x->count = x->count - y->count;

	// �� ��x�ĸ��ס� ��Ϊ ��y�ĸ��ס�
	y->parent = x->parent;

	if (x->parent == NULL)
	{
		root = y;            // ��� ��x�ĸ��ס� �ǿսڵ㣬��y��Ϊ���ڵ�
	}
	else
	{
		if (x->parent->left == x)
			x->parent->left = y;    // ��� x�������ڵ�����ӣ���y��Ϊ��x�ĸ��ڵ�����ӡ�
		else
			x->parent->right = y;    // ��� x�������ڵ�����ӣ���y��Ϊ��x�ĸ��ڵ�����ӡ�
	}

	// �� ��x�� ��Ϊ ��y�����ӡ�
	y->left = x;
	// �� ��x�ĸ��ڵ㡱 ��Ϊ ��y��
	x->parent = y;
	if (x->right != nullptr)//�ı�right_child��count��
		y->count = y->count - x->right->count + x->count;
	else
		y->count = y->count + x->count;
}

/*
 * �Ժ�����Ľڵ�(y)��������ת
 *
 */
template <class T>
void RBTree<T>::rightRotate(RBTNode<T>* &root, RBTNode<T>* y)
{
	// ����x�ǵ�ǰ�ڵ�����ӡ�
	RBTNode<T> *x = y->left;

	// �� ��x���Һ��ӡ� ��Ϊ ��y�����ӡ���
	// ���"x���Һ���"��Ϊ�յĻ����� ��y�� ��Ϊ ��x���Һ��ӵĸ��ס�
	y->left = x->right;
	if (x->right != NULL) {
		x->right->parent = y;
		//�ı�ptr��count��
		y->count = y->count - x->count + x->right->count;
	}
	else
		y->count = y->count - x->count;


	// �� ��y�ĸ��ס� ��Ϊ ��x�ĸ��ס�
	x->parent = y->parent;

	if (y->parent == NULL)
	{
		root = x;            // ��� ��y�ĸ��ס� �ǿսڵ㣬��x��Ϊ���ڵ�
	}
	else
	{
		if (y == y->parent->right)
			y->parent->right = x;    // ��� y�������ڵ���Һ��ӣ���x��Ϊ��y�ĸ��ڵ���Һ��ӡ�
		else
			y->parent->left = x;    // (y�������ڵ������) ��x��Ϊ��x�ĸ��ڵ�����ӡ�
	}

	// �� ��y�� ��Ϊ ��x���Һ��ӡ�
	x->right = y;

	// �� ��y�ĸ��ڵ㡱 ��Ϊ ��x��
	y->parent = x;
	if (y->left != nullptr)//�ı�left_child��count��
		x->count = x->count - y->left->count + y->count;
	else
		x->count = x->count + y->count;
}

/*
 * �����������������
 *
 * ���������в���ڵ�֮��(ʧȥƽ��)���ٵ��øú�����
 * Ŀ���ǽ������������һ�ź������
 *
 * ����˵����
 *     root ������ĸ�
 *     node ����Ľ��        // ��Ӧ���㷨���ۡ��е�z
 */
template <class T>
void RBTree<T>::insertFixUp(RBTNode<T>* &root, RBTNode<T>* node)
{
	RBTNode<T> *parent, *gparent;

	// �������ڵ���ڣ����Ҹ��ڵ����ɫ�Ǻ�ɫ��
	while ((parent = rb_parent(node)) && rb_is_red(parent))
	{
		gparent = rb_parent(parent);

		//�������ڵ㡱�ǡ��游�ڵ�����ӡ�
		if (parent == gparent->left)
		{
			// Case 1����������ڵ��Ǻ�ɫ
			{
				RBTNode<T> *uncle = gparent->right;
				if (uncle && rb_is_red(uncle))
				{
					rb_set_black(uncle);
					rb_set_black(parent);
					rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			// Case 2�����������Ǻ�ɫ���ҵ�ǰ�ڵ����Һ���
			if (parent->right == node)
			{
				RBTNode<T> *tmp;
				leftRotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			// Case 3�����������Ǻ�ɫ���ҵ�ǰ�ڵ������ӡ�
			rb_set_black(parent);
			rb_set_red(gparent);
			rightRotate(root, gparent);
		}
		else//����z�ĸ��ڵ㡱�ǡ�z���游�ڵ���Һ��ӡ�
		{
			// Case 1����������ڵ��Ǻ�ɫ
			{
				RBTNode<T> *uncle = gparent->left;
				if (uncle && rb_is_red(uncle))
				{
					rb_set_black(uncle);
					rb_set_black(parent);
					rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			// Case 2�����������Ǻ�ɫ���ҵ�ǰ�ڵ�������
			if (parent->left == node)
			{
				RBTNode<T> *tmp;
				rightRotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			// Case 3�����������Ǻ�ɫ���ҵ�ǰ�ڵ����Һ��ӡ�
			rb_set_black(parent);
			rb_set_red(gparent);
			leftRotate(root, gparent);
		}
	}

	// �����ڵ���Ϊ��ɫ
	rb_set_black(root);
}

/*
 * �������뵽�������
 *
 * ����˵����
 *     root ������ĸ����
 *     node ����Ľ��        // ��Ӧ���㷨���ۡ��е�node
 */
template <class T>
void RBTree<T>::insert(RBTNode<T>* &root, RBTNode<T>* node)
{
	RBTNode<T> *y = NULL;
	RBTNode<T> *x = root;

	// 1. �����������һ�Ŷ�������������ڵ���ӵ�����������С�
	while (x != NULL)
	{
		y = x;
		if (node->key < x->key) {
			x->count++;
			x = x->left;
		}
		else if (node->key > x->key) {
			x->count++;
			x = x->right;
		}
		else {
			x->count++;
			node->color = BLACK;
			return;//�������Ѿ�����key�Ľڵ�ʱ��ֱ�ӽ�count+1,Ȼ���˳������õ������ĺ�ڽṹ
		}
	}

	node->parent = y;
	if (y != NULL)
	{
		if (node->key < y->key)
			y->left = node;
		else
			y->right = node;
	}
	else
		root = node;

	// 2. ���ýڵ����ɫΪ��ɫ
	node->color = RED;

	// 3. ������������Ϊһ�Ŷ��������
	insertFixUp(root, node);
}

/*
 * �����(keyΪ�ڵ��ֵ)���뵽�������
 *
 * ����˵����
 *     tree ������ĸ����
 *     key ������ļ�ֵ
 */
template <class T>
void RBTree<T>::insert(T key)
{
	RBTNode<T> *z = NULL;

	// ����½����ʧ�ܣ��򷵻ء�
	if ((z = new RBTNode<T>(key, BLACK, NULL, NULL, NULL)) == NULL)
		return;

	insert(mRoot, z);
}

/*
 * �����ɾ����������
 * ����˵����
 *     root ������ĸ�
 *     node �������Ľڵ�
 */
template <class T>
void RBTree<T>::removeFixUp(RBTNode<T>* &root, RBTNode<T> *node, RBTNode<T> *parent)
{
	RBTNode<T> *other;

	while ((!node || rb_is_black(node)) && node != root)
	{
		if (parent->left == node)
		{
			other = parent->right;
			if (rb_is_red(other))
			{
				// Case 1: x���ֵ�w�Ǻ�ɫ��
				rb_set_black(other);
				rb_set_red(parent);
				leftRotate(root, parent);
				other = parent->right;
			}
			if ((!other->left || rb_is_black(other->left)) &&
				(!other->right || rb_is_black(other->right)))
			{
				// Case 2: x���ֵ�w�Ǻ�ɫ����w����������Ҳ���Ǻ�ɫ��
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			}
			else
			{
				if (!other->right || rb_is_black(other->right))
				{
					// Case 3: x���ֵ�w�Ǻ�ɫ�ģ�����w�������Ǻ�ɫ���Һ���Ϊ��ɫ��
					rb_set_black(other->left);
					rb_set_red(other);
					rightRotate(root, other);
					other = parent->right;
				}
				// Case 4: x���ֵ�w�Ǻ�ɫ�ģ�����w���Һ����Ǻ�ɫ�ģ�����������ɫ��
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->right);
				leftRotate(root, parent);
				node = root;
				break;
			}
		}
		else
		{
			other = parent->left;
			if (rb_is_red(other))
			{
				// Case 1: x���ֵ�w�Ǻ�ɫ��
				rb_set_black(other);
				rb_set_red(parent);
				rightRotate(root, parent);
				other = parent->left;
			}
			if ((!other->left || rb_is_black(other->left)) &&
				(!other->right || rb_is_black(other->right)))
			{
				// Case 2: x���ֵ�w�Ǻ�ɫ����w����������Ҳ���Ǻ�ɫ��
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			}
			else
			{
				if (!other->left || rb_is_black(other->left))
				{
					// Case 3: x���ֵ�w�Ǻ�ɫ�ģ�����w�������Ǻ�ɫ���Һ���Ϊ��ɫ��
					rb_set_black(other->right);
					rb_set_red(other);
					leftRotate(root, other);
					other = parent->left;
				}
				// Case 4: x���ֵ�w�Ǻ�ɫ�ģ�����w���Һ����Ǻ�ɫ�ģ�����������ɫ��
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->left);
				rightRotate(root, parent);
				node = root;
				break;
			}
		}
	}
	if (node)
		rb_set_black(node);
}

/*
 * ɾ�����(node)�������ر�ɾ���Ľ��
 *
 * ����˵����
 *     root ������ĸ����
 *     node ɾ���Ľ��
 */
template <class T>
void RBTree<T>::remove(RBTNode<T>* &root, RBTNode<T> *node)
{
	if (getSelfcount(node) > 1) {
		RBTNode<T> *p = node->parent;
		node->count--;
		while (p != NULL) {
			p->count--;
			p = p->parent;
		}
		return;
	}
	else {
		RBTNode<T> *child, *parent;
		RBTColor color;

		// ��ɾ���ڵ��"���Һ��Ӷ���Ϊ��"�������
		if ((node->left != NULL) && (node->right != NULL))
		{
			// ��ɾ�ڵ�ĺ�̽ڵ㡣(��Ϊ"ȡ���ڵ�")
			// ������ȡ��"��ɾ�ڵ�"��λ�ã�Ȼ���ٽ�"��ɾ�ڵ�"ȥ����
			RBTNode<T> *replace = node;

			// ��ȡ��̽ڵ�
			replace = replace->right;
			while (replace->left != NULL)
				replace = replace->left;

			//��������
			RBTNode<T> *p = rb_parent(replace);
			while (p != NULL) {
				p->count--;
				p = rb_parent(p);
			}
			// "node�ڵ�"���Ǹ��ڵ�(ֻ�и��ڵ㲻���ڸ��ڵ�)
			if (rb_parent(node))
			{
				//����λ��
				if (rb_parent(node)->left == node)
					rb_parent(node)->left = replace;
				else
					rb_parent(node)->right = replace;
			}
			else
				// "node�ڵ�"�Ǹ��ڵ㣬���¸��ڵ㡣
				root = replace;

			// child��"ȡ���ڵ�"���Һ��ӣ�Ҳ����Ҫ"�����Ľڵ�"��
			// "ȡ���ڵ�"�϶����������ӣ���Ϊ����һ����̽ڵ㡣
			child = replace->right;
			parent = rb_parent(replace);
			// ����"ȡ���ڵ�"����ɫ
			color = rb_color(replace);

			// "��ɾ���ڵ�"��"���ĺ�̽ڵ�ĸ��ڵ�"
			if (parent == node)
			{
				parent = replace;
			}
			else
			{
				// child��Ϊ��
				if (child)
					rb_set_parent(child, parent);
				parent->left = child;

				replace->right = node->right;
				rb_set_parent(node->right, replace);
			}

			replace->parent = node->parent;
			replace->color = node->color;
			replace->left = node->left;
			replace->count = node->count;
			node->left->parent = replace;

			if (color == BLACK)
				removeFixUp(root, child, parent);

			delete node;
			return;
		}

		if (node->left != NULL)
			child = node->left;
		else
			child = node->right;

		//��������
		RBTNode<T> *p = rb_parent(node);
		while (p != NULL) {
			p->count--;
			p = rb_parent(p);
		}

		parent = node->parent;
		// ����"ȡ���ڵ�"����ɫ
		color = node->color;

		if (child)
			child->parent = parent;

		// "node�ڵ�"���Ǹ��ڵ�
		if (parent)
		{
			if (parent->left == node)
				parent->left = child;
			else
				parent->right = child;
		}
		else
			root = child;

		if (color == BLACK)
			removeFixUp(root, child, parent);
		delete node;
	}
}

/*
 * ɾ��������м�ֵΪkey�Ľڵ�
 *
 * ����˵����
 *     tree ������ĸ����
 */
template <class T>
void RBTree<T>::remove(T key)
{
	RBTNode<T> *node;

	// ����key��Ӧ�Ľڵ�(node)���ҵ��Ļ���ɾ���ýڵ�
	if ((node = search(mRoot, key)) != NULL)
		remove(mRoot, node);
}

/*
 * ���ٺ����
 */
template <class T>
void RBTree<T>::destroy(RBTNode<T>* &tree)
{
	if (tree == NULL)
		return;

	if (tree->left != NULL)
		return destroy(tree->left);
	if (tree->right != NULL)
		return destroy(tree->right);

	delete tree;
	tree = NULL;
}

template <class T>
void RBTree<T>::destroy()
{
	destroy(mRoot);
}

/*
 * ��ӡ"���������"
 *
 * key        -- �ڵ�ļ�ֵ
 * direction  --  0����ʾ�ýڵ��Ǹ��ڵ�;
 *               -1����ʾ�ýڵ������ĸ���������;
 *                1����ʾ�ýڵ������ĸ������Һ��ӡ�
 */
template <class T>
void RBTree<T>::print(RBTNode<T>* tree, T key, int direction)
{
	if (tree != NULL)
	{
		if (direction == 0)    // tree�Ǹ��ڵ�
			cout << setw(2) << tree->key << "(B) is root" << endl;
		else                // tree�Ƿ�֧�ڵ�
			cout << setw(2) << tree->key << (rb_is_red(tree) ? "(R)" : "(B)") << " count= " << setw(2) << getSelfcount(tree) <<
			" is " << setw(2) << key << "'s " << setw(12) << (direction == 1 ? "right child" : "left child") << endl;

		print(tree->left, tree->key, -1);
		print(tree->right, tree->key, 1);
	}
}

template <class T>
void RBTree<T>::print()
{
	if (mRoot != NULL)
		print(mRoot, mRoot->key, 0);
}

template <class T>
int RBTree<T>::getSelfcount(RBTNode<T>* node) const {
	int l = node->left != NULL ? node->left->count : 0;
	int r = node->right != NULL ? node->right->count : 0;
	return node->count - l - r;
}

//��ȡkey�����еİ���С�����˳��λ�ã�
template <class T>
pair<int, int> RBTree<T>::rank(RBTNode<T> *root, T val) {
	if (root == NULL || root->key == val) {//�ݹ���ֹ����
		int low = -1;
		int hight = -1;
		if (root != nullptr) {
			if (root->left != nullptr)
				low = root->left->count;
			else
				low = 0;
			if (root->right != nullptr)
				hight = root->count - root->right->count - 1;
			else
				hight = root->count - 1;
		}
		pair<int, int> a(low, hight);
		return a;
	}
	if (val < root->key)
		return rank(root->left, val);
	else {
		pair<int, int> tmp = rank(root->right, val);
		if (tmp.first == -1)
			return tmp;
		else {
			int base = root->count - root->right->count;//����������ʱ��Ҫ�ݹ������������Ԫ�ظ���
			tmp.first = tmp.first + base;
			tmp.second = tmp.second + base;
			return tmp;
		}
	}
}


template <class T>
pair<int, int> RBTree<T>::rank(T val) {
	return rank(mRoot, val);
}

// ������λΪ k ��Ԫ��(index)
//pair�е�һ��Ԫ��Ϊ�����Ƿ�ɹ���־λ��0�����ҳɹ�����0�������쳣
template <class T>
pair<int, T> RBTree<T>::getK(RBTNode<T> *root, int index) {
	int flag = -1;
	int val = -1;
	if (index < 0)//�쳣����ֵ������ֵΪ�Ǹ���
		return pair<int, int>(flag, val);
	int low = -1;
	int hight = -1;
	if (root != nullptr) {
		if (index > root->count - 1)//�쳣����ֵ���������������Ͻ�
			return pair<int, int>(flag, val);

		if (root->left != nullptr)
			low = root->left->count;
		else
			low = 0;
		if (root->right != nullptr)
			hight = root->count - root->right->count - 1;
		else
			hight = root->count - 1;
	}
	else//�쳣
		return pair<int, int>(flag, val);

	if (index >= low && index <= hight) {
		val = root->key;
		flag = 0;
		return pair<int, int>(flag, val);
	}
	if (index < low)
		return getK(root->left, index);
	if (index > hight)
		return getK(root->right, index - hight - 1);//����������������ֵ��Ҫ��ȥ�������͸��ڵ�ļ�������

}
template <class T>
T& RBTree<T>::operator[](int index) {
	pair<int, T> tmp = getK(mRoot, index);
	if (tmp.first == 0)
		return tmp.second;
	else
		cout << "error��" << endl;
}


template <typename T>
	typename SplayTree<T>::Node* SplayTree<T>::rightRotate(Node* x) {
		Node* y  = x->left;
		x->left  = y->right;
		y->right = x;
		return y;
	}

	template <typename T>
	typename SplayTree<T>::Node* SplayTree<T>::leftRotate(Node* x) {
		Node* y  = x->right;
		x->right = y->left;
		y->left  = x;
		return y;
	}

	template <typename T>
	typename SplayTree<T>::Node* SplayTree<T>::splay(Node* root, std::size_t key) {
		// Base cases: root is NULL or key is present at root  
		if (root == nullptr || root->key == key)
			return root;

		// Key lies in left subtree  
		if (root->key > key) {
			// Key is not in tree, we are done  
			if (root->left == nullptr)
				return root;

			// Zig-Zig (Left Left)  
			if (root->left->key > key) {
				// First recursively bring the key as root of left-left  
				root->left->left = splay(root->left->left, key);

				// Do first rotation for root, second rotation is done after else  
				root = rightRotate(root);
			}
			else if (root->left->key < key) // Zig-Zag (Left Right)  
			{
				// First recursively bring the key as root of left-right  
				root->left->right = splay(root->left->right, key);

				// Do first rotation for root->left  
				if (root->left->right != nullptr)
					root->left = leftRotate(root->left);
			}

			// Do second rotation for root  
			return (root->left == nullptr) ? root : rightRotate(root);
		}
		else // Key lies in right subtree  
		{
			// Key is not in tree, we are done  
			if (root->right == nullptr)
				return root;

			// Zag-Zig (Right Left)  
			if (root->right->key > key) {
				// Bring the key as root of right-left  
				root->right->left = splay(root->right->left, key);

				// Do first rotation for root->right  
				if (root->right->left != nullptr)
					root->right = rightRotate(root->right);
			}
			else if (root->right->key < key)// Zag-Zag (Right Right)  
			{
				// Bring the key as root of right-right and do first rotation  
				root->right->right = splay(root->right->right, key);
				root               = leftRotate(root);
			}

			// Do second rotation for root  
			return (root->right == nullptr) ? root : leftRotate(root);
		}
	}

	template <typename T>
	void                         SplayTree<T>::preOrder(Node* node) {
		if (node != nullptr) {
			std::cout << node->key << " ";
			if (node->left)
				preOrder(node->left);
			if(node->right)
				preOrder(node->right);
		}
	}

	template <typename T>
	void SplayTree<T>::preOrder(Node* node, std::vector<T*>& list) {
		if (node != nullptr) {
			preOrder(node->left, list);
			list.push_back(node->value);
			preOrder(node->right, list);
		}
	}

	template <typename T>
	T*   SplayTree<T>::insert(std::size_t key, T* value) {
		++m_size;

		// Simple Case: If tree is empty  
		if (m_root == nullptr) {
			m_root = new Node(key, value);
			m_nodes.push_back(std::unique_ptr<Node>(m_root));
			return value;
		}
		// Bring the closest leaf node to root  
		m_root = splay(m_root, key);

		// If key is already present, then return  
		if (m_root->key == key) {
			--m_size;
			delete value;
			return m_root->value;
		}
		// Otherwise allocate memory for new node  
		auto newNode = new Node(key, value);

		// If root's key is greater, make root as right child of newNode and copy the left child of root to newNode  
		if (m_root->key > key) {
			newNode->right = m_root;
			newNode->left  = m_root->left;
			m_root->left   = nullptr;
		}

		// If root's key is smaller, make root as left child of newNode and copy the right child of root to newNode  
		else {
			newNode->left  = m_root;
			newNode->right = m_root->right;
			m_root->right  = nullptr;
		}

		m_root = newNode;
		m_nodes.push_back(std::unique_ptr<Node>(newNode));
		return value;
	}

	template <typename T>
	T* SplayTree<T>::insert(T* value) {
		return insert(m_size, value);
	}

	template <typename T>
	T* SplayTree<T>::search(std::size_t key) {
		m_root = splay(m_root, key);

		if (m_root)
			if (m_root->key == key)
				return m_root->value;

		return nullptr;
	}

	template <typename T>
	std::vector<T*> SplayTree<T>::getOrderedList() {
		std::vector<T*> returnList;
		preOrder(m_root, returnList);
		return returnList;
	}

	template <typename T>
	std::size_t          SplayTree<T>::size() {
		return m_size;
	}

	template <typename T>
	void   SplayTree<T>::print() {
		preOrder(m_root);
		std::cout << "\n";
	}

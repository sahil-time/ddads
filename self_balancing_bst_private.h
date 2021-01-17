



struct balanced_tree_node {
    long long key;	//Expect Client to provide this
    void* data;	//Satellite Data if any - Opaque for Server. Client will typecast and use as they want.
    struct balanced_tree_node* parent;
    struct balanced_tree_node* left;
    struct balanced_tree_node* right;
    int height;
};

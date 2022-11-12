#include "HybridTable.h"

Node::Node(int index, int val)
{
    // IMPLEMENT ME
    index_ = index;
    val_ = val;
}

Node::Node(int index, int val, Node* next)
{
    // IMPLEMENT ME
    index_ = index;
    val_ = val;
    next_ = next;
}

Node::~Node()
{
    // IMPLEMENT ME
    index_ = 0;
    val_ = 0;
    //delete next_;
    next_ = nullptr;
}

HybridTable::HybridTable()
{
    // IMPLEMENT ME
    array_ = new int[INITIAL_ARRAY_SIZE ] { 0, 0, 0, 0   };
    list_ = nullptr;
}

HybridTable::HybridTable(const int* p, int n)
{
    // IMPLEMENT ME
    array_ = new int[n] { 0 };
    ncntr_array = n;
    std::copy(p, p + n, array_);
    //delete list_
    list_ = nullptr;
}

HybridTable::~HybridTable()
{
    // IMPLEMENT ME
    delete[] array_;
    array_ = nullptr;
    delete list_;
    list_ = nullptr;
}

HybridTable::HybridTable(const HybridTable& other)
{
    list_ = nullptr;
    ncntr_array = other.getArraySize();
    // cout<<"copy constructor : ";
    // cout<<ncntr_array << "\n";
    array_ = new int[ncntr_array]{0};
    std::copy(other.array_, other.array_ + ncntr_array, array_);
    
   if(other.getTotalSize() > other.getArraySize())
   {
    list_ = deepClone(other.list_);
   }    
}

HybridTable& HybridTable::operator=(const HybridTable& other)
{
 if(this != &other)
 {
   list_ = nullptr;
    ncntr_array = other.getArraySize();
    // cout<<"copy constructor : ";
    // cout<<ncntr_array << "\n";
    array_ = new int[ncntr_array]{0};
    std::copy(other.array_, other.array_ + ncntr_array, array_);
    
   if(other.getTotalSize() > other.getArraySize())
   {
    list_ = deepClone(other.list_);
   } 
 }
    return *this;
}

int HybridTable::get(int i) const
{

    if (i < getArraySize()  && i > -1) {
        return array_[i];
    }
    else {
        Node* get_node = getInsertPosition(i, true);
        int num = get_node == nullptr ? 0 : get_node->val_;
        //return (const int)num;
        return num;
    }
    return 0;
}

void HybridTable::set(int i, int val)
{
    if (i > getArraySize()  || i < 0) 
    {
        Node* new_node = new Node(i, val);
        if (list_ != nullptr) 
        {
            //Node* prev_node = const_cast<Node*>(getInsertPosition(i, false));
            Node* prev_node = getInsertPosition(i, false);
            prev_node = prev_node == nullptr ? list_end : prev_node;
            if(prev_node->index_ < i)
            {
                new_node->next_ = prev_node->next_;
                prev_node->next_ = new_node;
                list_end = new_node;

            }
            else if(prev_node->index_  > i)
            {
                new_node->next_ =  list_;
                list_ = new_node;
            }
            else
            {
                prev_node->val_ = val;
            }
        }
        else {
            list_ = new_node;
            list_end  = list_;
        }
    }
    else {
        array_[i] = val;
    }

}

string HybridTable::toString() const
{
    //cout<<"into tostring method" <<"\n";
    string selements = "";
    Node* ptemp_list = list_;
    int nlist_refcounter = 0;
    
    //cout<<"getArraySize())" << getArraySize()<<"\n";

    while(nlist_refcounter < getArraySize())
    {
        selements += std::to_string(nlist_refcounter) + " : ";
        selements += std::to_string(array_[nlist_refcounter]) + "\n";

        nlist_refcounter++;
    }
   
    if(ptemp_list != nullptr)
    {
        selements += "---\n";
        while (ptemp_list != nullptr) {
            selements += std::to_string(ptemp_list->index_) + " : ";
            selements += std::to_string(ptemp_list->val_) ;
            ptemp_list = ptemp_list->next_;
            selements += ptemp_list != nullptr ? " --> " : "";
        }  
    }
    else
    {
        selements.replace(selements.size() -1 ,1,"");
    }
  
    return selements;
}

int HybridTable::getArraySize() const
{

    return ncntr_array;
}

int HybridTable::getTotalSize() const
{

    Node* ptemp_list = list_;
    int nlist_refcounter = 0;
    while (ptemp_list != nullptr) {
        ptemp_list = ptemp_list->next_;
        nlist_refcounter++;
    }
    int ntotal_count = ncntr_array + nlist_refcounter;
    return ntotal_count;
}

Node* HybridTable::getInsertPosition(int nIndex, bool findIndex) const
{

    Node* pforward_ptr = list_;

    Node * pbackward_ptr = nullptr;
    while (pforward_ptr != nullptr) {
        if (findIndex) {
            if (pforward_ptr->index_ == nIndex) {
                break;
            }
        }
        else {
            if (pforward_ptr->index_ > nIndex)
            {
                pforward_ptr = pbackward_ptr;
                break;
            }
            else if (pforward_ptr->index_ == nIndex)
            {
                break;
            }
        }
        pbackward_ptr = pforward_ptr;
        pforward_ptr = pforward_ptr->next_;

    }
    return pforward_ptr;
}

Node* HybridTable::deepClone(Node* phead_node) const
{
    Node* current = phead_node;
    Node* newList = nullptr;
    Node* tail = nullptr;

    while (current != nullptr)
    {
        if (newList == nullptr)
        {
            newList = new Node(current->index_,current->val_);
            tail = newList;
        }
        else 
        {
            tail->next_ = new Node (current->index_,current->val_) ; 
            tail = tail->next_;
        }
        current = current->next_;
    }
return newList;
}
#include "HybridTable.h"

//===Node Constructor=== 
Node::Node(int index, int val) :index_(index),val_(val),next_(nullptr) {}

//===Node Constructor=== 
Node::Node(int index, int val, Node* next): index_(index),val_(val),next_(next) {}

//===Node Destructor=== 
Node::~Node()
{
    next_ = nullptr;    
    val_ = 0;
    index_ = 0;
}

/*==================================
Function : HybridTable Constructor
==================================
Description : Initialize memory for array_ and  assigns list_ to nullptr */
HybridTable::HybridTable()
{
    array_ = new int[ng_Arraysize] { 0 };
    list_ = nullptr;
}

/*==================================
Function : HybridTable Parameterized Constructor
Arguments : *p (New array)  |  n (Size of New Array)
==================================
Description : Initialize memory for array_ copy the pointer p to array_ 
using standard copy method - Copy bytes from one mem loc to another
 and  assigns list_ to nullptr */
HybridTable::HybridTable(const int* p, int n)
{
    array_ = new int[n] { 0 };
    ng_Arraysize = n;
    std::copy(p, p + n, array_);
    list_ = nullptr;
}

/*==================================
Function : HybridTable Destructor
==================================
Description : Releases all memory of Node & aray_  Pointer and set it to nullptr */
HybridTable::~HybridTable()
{
    while(list_ != nullptr)
    {
        Node * del_refNode = list_;
        list_ = list_->next_;
        delete del_refNode;
        del_refNode = nullptr;
    }
    delete list_;
    list_ = nullptr;
    delete[] array_;
    array_ = nullptr;
}

/*==================================
Function : HybridTable Copy Constructor
Arguments : other (Address of HybridTable Object )
Return : HybridTable object
==================================
Description : Clones array_ and list_ */
HybridTable::HybridTable(const HybridTable& other)
{
    list_ = nullptr;
    ng_Arraysize = other.getArraySize();
    if(other.getTotalSize() > ng_Arraysize)
        list_ = clone_List_(other.list_);
    array_ = new int[ng_Arraysize]{0};
    std::copy(other.array_, other.array_ + ng_Arraysize, array_);
    
   
}

/*==================================
Function : HybridTable Operator overloading
Arguments : other ( Address of HybridTable Object  ) 
Return : HybridTable Object
==================================
Description : Clones array_ and list_ */
HybridTable& HybridTable::operator=(const HybridTable& other)
{
 if(this != &other)
 {
    list_ = nullptr;
    ng_Arraysize = other.getArraySize();
    if(other.getTotalSize() > ng_Arraysize)
        list_ = clone_List_(other.list_);
    array_ = new int[ng_Arraysize]{0};
    std::copy(other.array_, other.array_ + ng_Arraysize, array_);
 }
    return *this;
}

/*==================================
Function : get
Arguments : i - index of Element to be retrived
Return : Value of index i
==================================
Description : Retreives the Element from array_ / list_ */
int HybridTable::get(int i) const
{
    if (i < 0 || i >= getArraySize()) // from list
    {
        int Value = 0;
        Node* get_node = RetreiveNode(i);
        if(get_node == nullptr)
            Value = 0;
        else
            Value = get_node->val_;   
        get_node = nullptr;
        return Value;
    }
    else
        return array_[i]; // from array
    return 0;
}

/*==================================
Function : pushIntoList
Arguments : i - index , val - value
==================================
Description : Pushes the element sorted increasing order with index */

void HybridTable::pushIntoList(int i, int val)
{
    if(list_==nullptr) 
    {
        list_ = new Node(i, val);
        return;
    }
    else
    {
        Node* pItrNode = list_;
        Node* pPrevItrNode = nullptr;
        while(pItrNode != nullptr) 
        {
            if(i == pItrNode->index_) 
            {
                pItrNode->val_=val; break;
            }        
            else if(i < pItrNode->index_)
            {                
                if(pPrevItrNode != nullptr)
                    pPrevItrNode->next_=new Node(i, val, pItrNode);
                else
                   list_ = new Node(i, val, pItrNode); // Modify the head
                break;
            }
            else if(pItrNode->next_==nullptr) 
            {
                pItrNode->next_=new Node(i, val, nullptr);
                break;
            }
            pPrevItrNode=pItrNode;
            pItrNode=pItrNode->next_;
        }
        pPrevItrNode = nullptr; // Remove Reference
        pItrNode = nullptr;
    }    
}


/*==================================
Function : findPower
Arguments : index_ - Index of Element 
==================================
Description : Find the nearest power to the index i Eg : Index:6 ,Power:8   & Index : 10 ,Power : 16*/
int HybridTable::findPower(int index_)
{
 	int minPow  = 3;
	while (pow(BASEPOWER, minPow) <= index_)
        minPow++;
	minPow = pow(BASEPOWER, minPow);
    return minPow;
}

/*==================================
Function : RetreiveNode
Arguments : nInd - Index of Element to be retrived
==================================
Description : Retreives the Node Pointer if Element Exist or nullptr on the other case*/
Node* HybridTable::RetreiveNode(int nInd) const
{
    Node* pReflist = list_;
    while (pReflist != nullptr) 
    {
        if (pReflist->index_ == nInd) 
        {
            break;
        }
        pReflist = pReflist->next_;
    }
    return pReflist;
}



/*==================================
Function : getPostiveStartNode
Arguments : nPower - Power of 2 , i - Index of Element 
==================================
Description : Creates a new Node pointer which has onlu positive values lying withing the power range */
Node* HybridTable::RetreiveAllPostiveNode(int nPower, int i, int &PosEleCount)
{
    Node* pPNodes = nullptr;
    Node* pItr_PNodes = nullptr;
    Node* pItr_node = list_;

    while(pItr_node != nullptr)
    {
        if(pItr_node->index_ == i)
        {
            pPNodes = nullptr;
            break;
        }
        if(pItr_node->index_ >= ng_Arraysize && pItr_node->index_ < nPower)
        {
            PosEleCount++;            

            if(pPNodes == nullptr)
            {                
                pItr_PNodes = new Node(pItr_node->index_, pItr_node->val_);
                pPNodes = pItr_PNodes;
            }
            else
            {
                pItr_PNodes->next_ = new Node(pItr_node->index_, pItr_node->val_);
                pItr_PNodes = pItr_PNodes->next_;
            }
        }
        pItr_node = pItr_node->next_;
    }
    
    return pPNodes;
}

void HybridTable::setResizedArray(int i, int val,  Node* pPos_start , int nNewArraySize)
{
    int* new_array_ = new int[nNewArraySize -1 ]{0};
    std::copy(array_, array_ + ng_Arraysize, new_array_);

    ng_Arraysize = nNewArraySize;

        while(pPos_start != nullptr)
        {
            new_array_[pPos_start->index_] = pPos_start->val_;

        int indexCntr = 0;

        while(pPos_start != nullptr)
        {
            new_array_[pPos_start->index_] = pPos_start->val_;

            pPos_start = pPos_start->next_;
        }
    }
    new_array_[i] = val;
    array_ = new_array_;
}

bool HybridTable::checkifArrayCanResize(int i, int val)
{
    int nNewArraySize = findPower(i);

    int nPosEleCount = 0; // Holds the value of Total Number of Positive ELement in the list_
    Node* pPostive_Start = RetreiveAllPostiveNode(nNewArraySize, i, nPosEleCount);
    
    int ntentative_sum = ng_Arraysize +  1  + (pPostive_Start  == nullptr ? 0 : nPosEleCount);

    double dSizePerc = (double)ntentative_sum/nNewArraySize;
    
    if(dSizePerc >= 0.75)
    {
        setResizedArray(i, val, pPostive_Start , nNewArraySize );
        deleteList(pPostive_Start);
        return true;
    }    
    return false;
}

void HybridTable::deleteList(Node* pPos_StartNode)
{
    Node* pPosref = pPos_StartNode;
    while(pPosref != nullptr)
    {        
        deleteNode(pPosref->index_);
        pPosref = pPosref->next_;
    }
}

void HybridTable::deleteNode(int index)
{
    Node* ref = list_;
    Node* follow = nullptr;
    bool elementFound = false;
    while(ref != nullptr)
    {
        if(ref->index_ == index)
        {
            if(follow != nullptr)
            {
                follow->next_ = ref->next_; 
            }
            else
            {
                list_ = ref->next_;
            }
            elementFound = true;
            break;  
        }
        follow = ref;
        ref = ref->next_;
    }
  
    if(elementFound)
    {
        delete ref;
        ref = nullptr;
        
    }
}

void HybridTable::set(int i, int val)
{ 
    if(i > -1)
    {
        if(i < getArraySize())
        {
            array_[i] = val;
            return;
        }
        else
        {
           if(checkifArrayCanResize(i, val))
            {
               return; 
            }
        }
        
    }
    pushIntoList(i, val);
}

string HybridTable::toString() const
{
    string selements = "";
    Node* ptemp_list = list_;
    int nlist_refcounter = 0;

    while(nlist_refcounter < getArraySize())
    {
        selements += std::to_string(nlist_refcounter) ;
        selements += " : ";
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

    return ng_Arraysize;
}

int HybridTable::getTotalSize() const
{

    Node* ptemp_list = list_;
    int nlist_refcounter = 0;
    while (ptemp_list != nullptr) {
        ptemp_list = ptemp_list->next_;
        nlist_refcounter++;
    }
    int ntotal_count = ng_Arraysize + nlist_refcounter;
    return ntotal_count;
}

Node* HybridTable::clone_List_(Node* phead_node) const
{
    Node* current = phead_node;
    Node* newList = nullptr;
    Node* tail = nullptr;

    while (current != nullptr)
    {
        Node* New_Node = new Node(current->index_,current->val_);
        if (newList == nullptr)
        {
            newList = New_Node;
            tail = newList;
        }
        else 
        {
            tail->next_ = New_Node;
            tail = tail->next_;
        }
        current = current->next_;   
    }
return newList;
}
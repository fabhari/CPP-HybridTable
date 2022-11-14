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
Function : getPostiveStartNode
Arguments : nPower - Power of 2 , index - Index of Element , PosEleCount - total Positive element found within the range
==================================
Description : Creates a new Node pointer which has only positive values lying withing the power range */
Node* HybridTable::RetreiveAllPosNode(int nPower, int nIndex_, int &PosEleCount)
{
    if(list_==nullptr)return nullptr;
    Node* pos_List = nullptr;
    Node* Itr_Prev_List = nullptr;
    Node* itr_List = list_;
    do
    {
        if(itr_List->index_==nIndex_)
        {
            pos_List = nullptr;
            break;
        }
        if(itr_List->index_ >= ng_Arraysize)
        {
            if(itr_List->index_ < nPower)
            {                          
                Node* nNewNode =  new Node(itr_List->index_, itr_List->val_);
                if(pos_List != nullptr)
                {   
                    Itr_Prev_List->next_ = nNewNode;
                    Itr_Prev_List = Itr_Prev_List->next_;    
                }
                else
                {
                    Itr_Prev_List = nNewNode;
                    pos_List = Itr_Prev_List;
                }
                nNewNode = nullptr;
                PosEleCount++;  
            }
        }
        itr_List = itr_List->next_;
    }while(itr_List != nullptr);
    
    return  pos_List;
}

/*==================================
Function : get
Arguments : i - index of Element to be retrived
Return : Value of index i
==================================
Description : Retreives the Element from array_ / list_ */
int HybridTable::get(int nIndex_) const
{
    if (nIndex_ < 0 || nIndex_ >= getArraySize()) // from list
    {
        int Value = 0;
        Node* get_node = RetreiveNode(nIndex_);
        if(get_node == nullptr)
            Value = 0;
        else
            Value = get_node->val_;   
        get_node = nullptr;
        return Value;
    }
    else
        return array_[nIndex_]; // from array
    return 0;
}

/*==================================
Function : pushIntoList
Arguments : nIndex_ - index , val - value
==================================
Description : Pushes the element sorted increasing order with index */

void HybridTable::pushIntoList(int nIndex_, int val)
{
    if(list_==nullptr) 
    {
        list_ = new Node(nIndex_, val);
        return;
    }
    else
    {
        Node* pItrNode = list_;
        Node* pPrevItrNode = nullptr;
        do
        {
            if(pItrNode->index_ == nIndex_) 
            {
                pItrNode->val_=val; break;
            }        
            else if(pItrNode->index_ > nIndex_)
            {                
                if(pPrevItrNode != nullptr)
                    pPrevItrNode->next_=new Node(nIndex_, val, pItrNode);
                else
                   list_ = new Node(nIndex_, val, pItrNode); // Modify the head
                break;
            }
            else if(pItrNode->next_==nullptr) 
            {
                pItrNode->next_=new Node(nIndex_, val, nullptr);
                break;
            }
            pPrevItrNode=pItrNode;
            pItrNode=pItrNode->next_;
        }while(pItrNode != nullptr);
        pPrevItrNode = nullptr; // Remove Reference
        pItrNode = nullptr;
    }    
}


/*==================================
Function : findPower
Arguments : nIndex_ - Index of Element 
==================================
Description : Find the nearest power to the index i Eg : Index:6 ,Power:8   & Index : 10 ,Power : 16*/
int HybridTable::findPower(int nIndex_)
{
 	int minPow  = 3;
	while (pow(BASEPOWER, minPow) <= nIndex_)
        minPow++;
	minPow = pow(BASEPOWER, minPow);
    return minPow;
}

/*==================================
Function : RetreiveNode
Arguments : nInd - Index of Element to be retrived
==================================
Description : Retreives the Node Pointer if Element Exist or nullptr on the other case*/
Node* HybridTable::RetreiveNode(int nIndex_) const
{
    if(list_==nullptr)return nullptr;
    Node* pReflist = list_;
    do
    {
        if (pReflist->index_ == nIndex_) 
        {
            break;
        }
        pReflist = pReflist->next_;
    } while (pReflist != nullptr) ;
    return pReflist;
}

/*==================================
Function : ExtendArrayMemory
Arguments : index - Index of Element , val - Value, pPostart - Pointer of positve list , 
            nNewArraySize - New array Size
==================================
Description : Extends the array_ memeory , shift the elements from list_ to array_*/
void HybridTable::ExtendArrayMemory(int index_, int val,  Node* p_Node_ , int nNewArraySize)
{
    if(p_Node_ == nullptr)return;
    int* pNew_array_ = new int[nNewArraySize -1 ]{0};
    pNew_array_[index_] = val;
    std::copy(array_, array_ + ng_Arraysize, pNew_array_);
    ng_Arraysize = nNewArraySize;
    int indexCntr = 0;
    do
    {
        pNew_array_[p_Node_->index_] = p_Node_->val_;
        p_Node_ = p_Node_->next_;
    }while(p_Node_ != nullptr);
    array_ = pNew_array_;
}

/*==================================
Function : DropNodeElement
Arguments : index - Index of Element 
==================================
Description : Drop a node from list and update it*/
void HybridTable::DropNodeElement(int index)
{
    if(list_ == nullptr)return;
    Node* ItrForwardNode = nullptr;
    Node* ItrNode = list_; 
    do
    {
        if(ItrNode->index_ == index)
        {
            if(ItrForwardNode != nullptr)
                ItrForwardNode->next_ = ItrNode->next_; 
            else
                list_ = ItrNode->next_;
            delete ItrNode;
            ItrNode = nullptr;  
            break;  
        }
        ItrForwardNode = ItrNode;
        ItrNode = ItrNode->next_;
    } while(ItrNode != nullptr);  
}

/*==================================
Function : set
Arguments : index_ - Index of Element , val - value of Node
==================================
Description : Drop a node from list and update it*/
void HybridTable::set(int index_, int val)
{ 
    if(index_ > -1)
    {
        if(index_ >= getArraySize())
        {
            int nNewArraySize = findPower(index_);
            int nPosEleCount = 0; // Holds the value of Total Number of Positive ELement in the list_
            Node* pPostive_Start = RetreiveAllPosNode(nNewArraySize, index_, nPosEleCount);
            int ntentative_sum = ng_Arraysize +  1  + nPosEleCount;
            float dSizePerc = (float)ntentative_sum/nNewArraySize;
            if(dSizePerc >= 0.75)
            {
            ExtendArrayMemory(index_, val, pPostive_Start , nNewArraySize );
            Node* pPosref = pPostive_Start;
            while(pPosref != nullptr)
            {        
                DropNodeElement(pPosref->index_);
                pPosref = pPosref->next_;
            }
            return;
            }          
        }
        else
        {
             array_[index_] = val;
             return;
        }        
    }
    pushIntoList(index_, val);
}
/*==================================
Function : clone_List_
Arguments : phead_node - Pointer of list to be cloned
==================================
Description : Gets Array Size + List_ size ..*/
Node* HybridTable::clone_List_(Node* pNode_Start) const
{
    if(pNode_Start == nullptr)return nullptr;
    Node* NodeTail = nullptr;
    Node* ItrNode = pNode_Start;
    Node* NodeHead = nullptr;

    do{
        Node* New_Node = new Node(ItrNode->index_,ItrNode->val_);
        if (NodeHead != nullptr)
        {
            NodeTail->next_ = New_Node;
            NodeTail = NodeTail->next_;
        }
        else 
        {
            NodeHead = New_Node;
            NodeTail = NodeHead;
        }
        ItrNode = ItrNode->next_;   
    }while (ItrNode != nullptr);
return NodeHead;
}

/*==================================
Function : toString
Arguments : None
==================================
Description : Converts all array_ and node elements in the form of string..*/
string HybridTable::toString() const
{
    int nArrayCntr = 0;
    string sFinalstr = ""; 
    do
    {
        sFinalstr += std::to_string(nArrayCntr);
        sFinalstr += " : ";
        sFinalstr += std::to_string(array_[nArrayCntr]);
        sFinalstr += "\n";
        nArrayCntr++;
    }while(nArrayCntr < getArraySize());

    if(list_ == nullptr)
        sFinalstr.replace(sFinalstr.size() -1 ,1,"");
    else
    {
        Node* ItrList_ = list_;
        sFinalstr += "---\n";
        do{
            sFinalstr += std::to_string(ItrList_->index_) ;
            sFinalstr += " : ";
            sFinalstr += std::to_string(ItrList_->val_) ;
            ItrList_ = ItrList_->next_;
            if(ItrList_ != nullptr)
                sFinalstr += " --> ";
            else
                sFinalstr += "";
        } while (ItrList_ != nullptr);
    }
        
    return sFinalstr;
}

/*==================================
Function : getArraySize
Arguments : None
==================================
Description : Gets Array Size..*/
int HybridTable::getArraySize() const
{
    return ng_Arraysize;
}


/*==================================
Function : getTotalSize
Arguments : None
==================================
Description : Gets Array Size + List_ size ..*/
int HybridTable::getTotalSize() const
{
    if(list_ == nullptr) return ng_Arraysize;
    Node* ItrList_ = list_;
    int nItrCounter = 0;
    do{
        ItrList_ = ItrList_->next_;
        nItrCounter++;
    } while (ItrList_!= nullptr) ;
    return (ng_Arraysize + nItrCounter);
}



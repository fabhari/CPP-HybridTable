#include "HybridTable.h"

Node::Node(int index, int val)
{
    // IMPLEMENT ME
    index_ = index;
    val_ = val;
    next_ = nullptr;
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
        return num;
    }
    return 0;
}

Node* HybridTable::getInsertPosition(int nIndex, bool findIndex) const
{

    Node* pforward_ptr = list_;

    while (pforward_ptr != nullptr) 
    {
        if (pforward_ptr->index_ == nIndex) 
        {
            break;
        }
        pforward_ptr = pforward_ptr->next_;
    }
    return pforward_ptr;
}

void HybridTable::setList(int i, int val)
{
    if (list_ != nullptr) 
    {
        Node* newNode = nullptr;
        Node* refList = list_;
        Node* prevList = nullptr;

        while (refList != nullptr) 
        {
            if (i == refList->index_ ) 
            {
                refList->val_ = val;
                break;
            }        
            else if( i < refList->index_ )
            {
                newNode = new Node(i, val, refList);
                if(prevList != nullptr)
                {
                    prevList->next_ = newNode;
                }
                else
                {
                   list_ = newNode;
                }
               break;
            }
            else if (refList->next_ == nullptr) 
            {
                refList->next_ = new Node(i, val, nullptr);
                break;
            }
            prevList = refList;
            refList = refList->next_;
        }
    }
    else 
    {
        list_ = new Node(i, val);
    }
}

void HybridTable::setArray(int i, int val)
{
    array_[i] = val;
}

int HybridTable::getNearestPower(int i)
{
    int npower = 2;
    int nMaxPower = -1;
    while(true)
    {
        nMaxPower = pow(2,npower) ;//2 ^ npower;
        if(i < nMaxPower)  
        {
            break;
        }
        npower++;
    }
    return nMaxPower;
}

Node* HybridTable::getPostiveStartNode(int nPower, int i)
{
    Node* pPNodes = nullptr;
    Node* pItr_PNodes = nullptr;

    npositiveElement = 0;
    Node* pItr_node = list_;

    while(pItr_node != nullptr)
    {
        if(pItr_node->index_ == i)
        {
            pPNodes = nullptr;
            break;
        }
        if(pItr_node->index_ >= ncntr_array && pItr_node->index_ < nPower)
        {
            npositiveElement++;            

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

void HybridTable::setResizedArray(int i, int val,  Node* pPos_start , int nNewSize)
{
    int* new_array_ = new int[nNewSize -1 ]{0};

    std::copy(array_, array_ + ncntr_array, new_array_);

    ncntr_array = nNewSize;
    delete array_;
    array_ = nullptr;

    if(pPos_start != nullptr)
    {
        int indexCntr = 0;

        cout<<"\nNew Element : " << i <<" = ";

        while(pPos_start != nullptr)
        {
            new_array_[pPos_start->index_] = pPos_start->val_;

            cout<<pPos_start->index_<<" " ;

            pPos_start = pPos_start->next_;
        }
    }
    new_array_[i] = val;
    array_ = new_array_;
}


bool HybridTable::checkifArrayCanResize(int i, int val)
{
    int nNewSize = getNearestPower(i);

    Node* pPostive_Start = getPostiveStartNode(nNewSize, i);
    
    int ntentative_sum = ncntr_array +  1  + (pPostive_Start  == nullptr ? 0 : npositiveElement);

    cout<<"\nMax Power: "<< i <<" : "<< nNewSize <<"\n";
    cout<<"\nTen_sum  : "<< i <<" : "<< ntentative_sum <<"\n";
    double dSizePerc = (double)ntentative_sum/nNewSize;
    
    if(dSizePerc >= 0.75)
    {
        cout<<"\nResizing the array\n";
        setResizedArray(i, val, pPostive_Start , nNewSize );
        return true;
    }    
    return false;
}

void HybridTable::deleteNode(int index)
{
    cout<<list_<<"\n";
    Node* ref = list_;
    Node* follow = list_;
    Node* temp = nullptr;
    while(ref != nullptr)
    {
        if(ref->index_ == index)
        {
           temp = ref;
            if(ref->next_!=nullptr)
            {                
                follow->next_ = ref->next_;                    
            }
            break;  
        }
        follow = ref;
        ref = ref->next_;
    }
    cout<<temp->index_<<"\n";
    cout<<temp->val_<<"\n";
    delete temp;  
    temp = nullptr; 
    cout<<"Memory of temp : "<<temp;
    cout<<"Memory of list_ : "<<list_->next_;
}

void HybridTable::set(int i, int val)
{
   
    if(i > -1)
    {
        if(i < getArraySize())
        {
            setArray(i, val);
            return;
        }
        else
        {
        //     cout<<"\n\n =================\nSET METHOD\n=============\n";
        //    if(checkifArrayCanResize(i, val))
        //     {
        //         cout<<"\nTrying to Resize Array\n";
        //         cout<<"\n\n New Element Added : \n"<<toString() <<"\n-- End --\n\n";
        //        return; 
        //     }
        }
        
    }
    //cout<<"\n\n Before Element Added : \n"<<toString() <<"\n-- End --\n\n";
    setList(i, val);
      //cout<<"after of list_ : "<<list_->next_;
    //deleteNode(i);
    //cout<<"\nAfter Element Added : \n"<<toString() <<"\n-- End --\n\n";
}

string HybridTable::toString() const
{
    string selements = "";
    Node* ptemp_list = list_;
    int nlist_refcounter = 0;

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
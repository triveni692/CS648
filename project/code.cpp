#include <bits/stdc++.h>

using namespace std;

#define      pii               std::pair<int,int>
#define      vi                std::vector<int>
#define      mp(a,b)           std::make_pair(a,b)
#define      pb(a)             push_back(a)
#define      each(it,s)        for(auto it = s.begin(); it != s.end(); ++it)
#define      rep(i, n)         for(int i = 0; i < (n); ++i)
#define      fill(a)           memset(a, 0, sizeof (a))
#define      sortA(v)          sort(v.begin(), v.end())
#define      sortD(v)          sort(v.begin(), v.end(), greater<auto>())
#define      X                 first
#define      Y                 second

typedef long long LL;
typedef unsigned long long ULL;

const int INF = 1000000001;

// Find Hash value for `key`
inline int getHash(LL a, LL b, int p, int size, int key)
{
    return (((a * key + b) % p) % size);
}

// class for mini-hash table in cells of main hash-table 
class Bucket
{
public:
    vector<int> _cells; // Hash table
    int size;           // size of the hash table
    LL hash_a;          // H(key) = (hash_a * key + hash_b) % prime
    LL hash_b;
    int prime;

    Bucket() {}
    void clear() {_cells.clear();size = 0;}
    void Initialize()   // This function initializes the necessary values
    {
        prime = 1000000007;
        hash_a = rand() % prime;
        hash_b =  1 + rand() % (prime - 1);
    }

    // construct hash table from list of elements
    // Variable `flag` makes sure that this hash table do not have any collisions
    // See line number 74-78.
    void Construct(list<int>& input)
    {
        if (input.empty())
        {
            size = 0;
            return;
        }

        size = input.size() * input.size();
        bool flag = true;

        // While there is no collisions in the table table, make the hash table
        // and if a collision occurs, break from the loop and re-construct table
        // with different values of hashing parameters, hash_a, hash_b and prime
        while (flag)
        {
            _cells.assign(size, INF);   // INF means that that particular cell hash not been used till now
            Initialize();
            list<int>::iterator elem = input.begin();
            while (elem != input.end() && flag)
            {
                int hashKey = getHash(hash_a, hash_b, prime, size, *elem);
                if (hashKey < 0) 
                    hashKey = - hashKey;

                // if collision then construct hash table from the begining!
                if (_cells[hashKey] != INF) 
                {
                    flag = false;
                    break;
                }
                _cells[hashKey] = *elem;
                ++elem;
            }

            if (!flag)
                flag = true;
            else 
                flag = false;
        }
    }
    // This function chechs if `elem` is present in the hash table or not in O(1) time
    bool Contains(int elem)
    {
        if (size == 0)
            return false;
        int hashKey = getHash(hash_a, hash_b, prime, size, elem);
        if (hashKey < 0) 
            hashKey = - hashKey;
        if (_cells[hashKey] == elem)
            return true;
        return false;
    }
};

// This class takes an array of integers as argument and creates hash table
// with N + o(N) total memory with worst case search time O(1) guarantee.
class GoodHashing
{
public:
    vector<int> elements;               // This is the main hash table, this same as the array T[] explained in the paper / slides
    int gn;                             // This is size of the primary hash table / first level hash table, array A[] in the slides
    int N;                              // N elements to be hashed
    int prime;                          // H(key) = (hash_a * key + hash_b) % prime
    LL hash_a, hash_b;
    vector<ULL> isEmpty;                // `i`th bit is 0, if there is no element in the input with first level hash value = `i`
    vector<ULL> isSingleTon;            // `i`th bit is 0, if there is a single element in the input with first level hash value = `i`
                                        // Please note that here convention of 0/1 is opposite to what hash been explained in the slides

    vector<int> baseAdd;                // This is the array containing base address of indices at an interval of 64, this is B[] in the slides
    vector<char> offset;                // This is the array A[] as explained in the slides. It contains an `offset` for each index `i` such that
                                        // elements[baseAdd[] + offset[i]] points to the actual key for an element hashed primarily to `i`
    GoodHashing(){
        prime = 1000000009;             // the maximum prime number used for first level hashing
        hash_a = rand() % prime;        // hash_a and hash_b are as explained above, are hashing parameter for first level
        hash_b = 1 + rand()%(prime-1);  // As soon as this class is declared, these values get set automatically
    }
    void Initialize(const vector<int>& input){ // input is the input array to be hashed 
        N = input.size();
        gn = N * sqrt(log2(N));                 // gn = g(n) = N * sqrt(log(N)) as described in the paper / slides

        isEmpty.resize(gn/64 + !!(gn%64));      // size of the arrays isEmpty and isSingleTon is N/64 unsigned long ints 
        isSingleTon.resize(gn/64 + !!(gn%64));  // our algorithm uses exactly `one bit` for each 0 <= `i` < gn, total memory <= N^2 / gn = o(N)

        rep(i, isEmpty.size()) isEmpty[i] = isSingleTon[i] = 0;

        map<int, int> st;
        // In the following loop, which indices `i` are empty and which are singleTon is detected first so that
        // accordingly second level hash table will be formed. Intially it assumes that every index is empty and as soon
        // as some element falls into this bucket, corresponding bit is updated. Similarily all the indices are assumed
        // singleTon initially then if any bucket gets more that 1 element, corresponding bit is updated. 
        rep(i, N){
            int hashKey = getHash(hash_a, hash_b, prime, gn, input[i]);
            if (hashKey < 0) 
                hashKey = - hashKey;
            assert(hashKey >= 0 && hashKey < gn);
            int x = hashKey / 64;
            int y = hashKey % 64;
            if((isEmpty[x]&(1ULL<<y)) == 0ULL) { // if `A[hashKey]` was empty till now butan hashKey falls into this bucket, it is updated
                isEmpty[x] |= (1ULL<<y);
            }
            else {                              // if A[hashKey] was not empty till now but singleTon, corresponding bit is updated as 
                                                // new element,input[i], falls into this bucket
                if((isSingleTon[x]&(1ULL<<y)) == 0ULL) st.insert({hashKey,0});
                isSingleTon[x] |= (1ULL<<y);
            }
        }
        
        int itemp = 0;      // it is count of how many buckets are there which are not singleTon,
                            //  meaning how many second level hash tables we need to create
        for(auto &it : st){
            it.Y = itemp++;
        }
        vector<list<int> > inp(itemp);  // this temporary 2-D array keeps the elements which need to be hashed with second level
                                        // Following for loop does that job
        rep(i, N){
            int hashKey = getHash(hash_a, hash_b, prime, gn, input[i]);
            if (hashKey < 0) 
                hashKey = - hashKey;
            
            int x = hashKey / 64;
            int y = hashKey % 64;
               
            if(isSingleTon[x]&(1ULL<<y)) {
                inp[st[hashKey]].push_back(input[i]);
            }
        }
        Bucket bct;
        itemp = 0;
        offset.resize(gn);

        // following for loop does most of the important work. For each index 0 <= `i` < gn ,
        // 1. If an bucket is empty             : do nothing and straighaway go to next index
        // 2. If a bucket if singleton          : push `-1` for now and continue, exact key will be pushed later 
        // 3. If it contains multiple entries   : Create a second level hash table and then push each element
        //                                        of the second level hash table into array elements. But before that push the 4 necessary 
        //                                        parameters- hash_table_size, hash_a, hash_b and prime into the elements array so that 
        //                                        required hashing information can be extracted later while doing queries

        // Parallelly update the base address array and the offset array as well. 
        rep(i, gn){
            int x = i / 64, y = i % 64;
            if(y == 0){
                baseAdd.push_back(elements.size());             // updating base address array
                offset[i] = 0;
            }
            else {
                offset[i] = (int)elements.size() - baseAdd[x];  // updating the offset array
            }
            if(!(isEmpty[x]&(1ULL<<y))) continue;               // case (1) when the bucket is empty
            if(!(isSingleTon[x]&(1ULL<<y))) elements.pb(-1);    // case (2) when the bucket is singlTon
            else{                                               // case (3) create the second level hash
                bct.Initialize();
                bct.Construct(inp[itemp++]);
                elements.push_back(bct.size);                   // push the necessary 4 elements into the array
                elements.push_back(bct.hash_a);
                elements.push_back(bct.hash_b);
                elements.push_back(bct.prime);
                assert(bct.size == bct._cells.size());
                rep(ii, bct.size){
                    elements.push_back(bct._cells[ii]);         // push the hash table of second level 
                }
                bct.clear();
            }
        }
        // complete the necessary work pending for singleTon elements 
        // detect which element is singleTon. Then find the base address 
        // add the offset value and reach to the corresponding index.
        // There will be an `-1` sitting in place of the actual key. Update it.
        rep(i, N){
            int hashKey = getHash(hash_a, hash_b, prime, gn, input[i]);
            if (hashKey < 0) 
                hashKey = - hashKey;
            
            int x = hashKey / 64;
            int y = hashKey % 64;
            if(isEmpty[x]&(1LL<<y)) {
                if(!(isSingleTon[x]&(1ULL<<y))) elements[baseAdd[hashKey/64]+offset[hashKey]] = input[i]; // update the -1 with actual key value
            }
        }
       
    }
    // making query in O(1) worst case time
    // first get the primary hashKey.
    // 1. if hashKey corresponds to empty : return 'Not Found'
    // 2. if hashKey corresponds to single : get the actual_address = base address + offset value, check if elements[actual_address] = key.
    //                                       return accordingly.
    // 3. if hashkey corresponds to bucket size > 1 : From the actual_address find the first 4 parameters used for second level hashing.
    //                                                using those parameters find the second level hash value. From that hash value go to the
    //                                                proper address, by adding that hash value to actual address, and check if the key sitting
    //                                                there is equal to the key we are searching. Return accordingly
    bool Contains(int elem){
        int hashKey = getHash(hash_a, hash_b, prime, gn, elem);
        if (hashKey < 0) 
            hashKey = - hashKey;
        // hashKey  is first level hash value
        int x = hashKey / 64;
        int y = hashKey % 64;
        
        if(!(isEmpty[x]&(1LL<<y))) return 0; // // case 1 - 'Not Found'
        
        int add = baseAdd[x]+offset[hashKey]; // actual address
        if(!(isSingleTon[x]&(1ULL<<y))) return elements[add] == elem; // case 2, singleTon

        // get the second level hash value
        int newHashKey = getHash(elements[add+1], elements[add+2], elements[add+3], elements[add], elem);
        return elements[add + 4 + newHashKey] == elem;
    }
    // This functions retuns the total amount of memory, not temporary which is very less, used in the whole process
    // returned value is in the form of number of integers, assuming one integer = 8 byte
    int getMemory(){
        return (offset.size() + baseAdd.size() * 8 + N * 8 + (isSingleTon.size() + isEmpty.size()) * 16) / 8;
    }
};

// Driver function
int main()
{
    // srand (time(NULL));
    int N;
    cin >> N;

    GoodHashing sh;
    vector<int> input;
    
    for (int i = 0; i < N; ++i)
    {
        int x;
        scanf("%d", &x);
        input.push_back(x);
    }

    sh.Initialize(input);
    int Q;
    cin >> Q;
    cout << sh.getMemory() << endl;
    return 0;
    for (int i = 0; i < Q; ++i)
    {
        int x;
        scanf("%d", &x);
        puts(sh.Contains(x) ? "Yes" : "No");
    }
    return 0;
}

About : This repo contains code for Hashing an array of size N with worst case search time guarantee of O(1). Total memory used by the code is N + o(N).  

How to use : 
	1. Code is written in c++. 
	2. Compile it using flag -std=c++0x. Example - 'g++ code.cpp -o code -std=c++0x". 
	3. file `simple.cpp` uses simple hashing technique. 
	4. file `code.cpp` actually impliments 'Perfect Hashing' with space optimization.
	5. To use it in a project - copy paste the whole code. Create a vector of elements to be hashed. Declare a class GoodHashing. Example - GoodHashing hs. Pass the vector as an argument. 
	6. Then for query, sh.Contains(key) will return 1 if the key is present, otherwise returns zero. A driver function is written in the code to understand it better.
	7. This class works very well for N <= 10,000,000.

Reference and Acknowledgement :
1. We thank Prof. Surender Baswana (homepage : http://www.cse.iitk.ac.in/users/sbaswana/) for teaching and guiding us while doing this project. His lectures were very inspirational to us. It is him, because of which we are able to do whatever we did.

2. we read this - http://www.cs.dartmouth.edu/~ac/Teach/CS105-Winter05/Handouts/fks-perfecthash.pdf for memory optimizations part.
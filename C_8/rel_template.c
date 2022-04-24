#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair* A, int size, pair newPair){
    int isHere = 0;
    for (int i = 0; i < size; i++){
        if(newPair.first == A[i].first && newPair.second == A[i].second)
            isHere = 1;
    }
    if(isHere == 0){
        A[size] = newPair;
        return size + 1;
    }
    return size;
}


// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair* A, int size){
    int isReflexive = 1;
    for(int i = 0; i < size; i++){
        int isReflexiveFirst = 0, isReflexiveSecond = 0;
        for (int j = 0; j < size; j++){
            if(A[i].first == A[j].first && A[i].first == A[j].second)
                isReflexiveFirst = 1;
            if(A[i].second == A[j].first && A[i].second == A[j].second)
                isReflexiveSecond = 1;
        }
        if(isReflexiveFirst == 0 || isReflexiveSecond == 0) {
            isReflexive = 0;
            break;
        }
    }
    return isReflexive;
}
// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair* A, int size){
    int isIrreflexive = 1;
    for(int i = 0; i < size; i++){
        int isReflexiveFirst = 0, isReflexiveSecond = 0;
        for (int j = 0; j < size; j++){
            if(A[i].first == A[j].first && A[i].first == A[j].second)
                isReflexiveFirst = 1;
            if(A[i].second == A[j].first && A[i].second == A[j].second)
                isReflexiveSecond = 1;
        }
        if(isReflexiveFirst == 1 || isReflexiveSecond == 1) {
            isIrreflexive = 0;
            break;
        }
    }
    return isIrreflexive;
}
// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair* A, int size){
    int isSymmetric = 1;
    for(int i = 0; i < size; i++){
        int isSymmetricPair = 0;
        for (int j = 0; j < size; j++){
            if(A[i].first == A[j].second && A[i].second == A[j].first)
                isSymmetricPair = 1;

        }
        if(isSymmetricPair == 0) {
            isSymmetric = 0;
            break;
        }
    }
    return isSymmetric;
}
// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair* A, int size){
    int isAntiSymmetric = 1;
    for(int i = 0; i < size; i++){
        if(A[i].first == A[i].second)
            continue;
        int isSymmetricPair = 0;
        for (int j = 0; j < size; j++){
            if(A[i].first == A[j].second && A[i].second == A[j].first)
                isSymmetricPair = 1;

        }
        if(isSymmetricPair == 1) {
            isAntiSymmetric = 0;
            break;
        }
    }
    return isAntiSymmetric;
}
// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair*A, int size){
    if(is_antisymmetric(A, size) == 1 && is_irreflexive(A, size) == 1)
        return 1;
    return 0;
}
// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair* A, int size){
    int isTransitive = 1;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if(i == j || A[i].second != A[j].first)
                continue;
            int isTransitivePairs = 0;
            for(int k = 0; k < size; k++)
                if(A[i].first == A[k].first && A[j].second == A[k].second)
                    isTransitivePairs = 1;
            if (isTransitivePairs == 0){
                //isTransitive =0;
                return 0;
            }
        }
    }
    return 1;
}

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair*A, int size){
    if(is_reflexive(A, size) == 1 && is_transitive(A, size) == 1 && is_antisymmetric(A, size ) == 1)
        return 1;
    return 0;
}

int is_pair(pair * A, int size, int x, int y){
    //int is__make_pair = 0;
    for (int i = 0; i < size ; i++)
        if((A[i].first == x && A[i].second == y) || (A[i].first == y && A[i].second == x))
            return 1;
    return 0;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair* A, int size){
    for(int i = 0; i < size; i++){
        for (int j = i+1; j < size; ++j) {
            if(is_pair(A, size, A[i].first, A[j].first) == 0 ||
                    is_pair(A, size, A[i].first, A[j].second) == 0 ||
                    is_pair(A, size, A[i].second, A[j].first) == 0 ||
                    is_pair(A, size, A[i].second, A[j].second) == 0)
                return 0;
        }
    }
    return 1;
}

// A total order relation is a partial order relation that is connected
int is_total_order(pair* A, int size){
    if(is_partial_order(A, size) == 1 && is_connected(A, size))
        return 1;
    return 0;
}

int add_to_array(int* Array, int size, int x){
    int isThere = 0;
    for(int i = 0; i < size; i++)
        if(Array[i] == x)
            isThere = 1;
    if (isThere == 0)
        Array[size++] = x;
    return size;
}

void bubble_sort(int* arr, int size){
    for(int i = 0; i < size; i++) {
        for (int j = 0; j < size - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

int find_max_elements(pair* A, int size, int* Result){
    int resultSize = 0;
    for(int i = 0; i < size; i++){
        int isMax = 1;
        for (int j = 0; j < size; j++){
            if (A[i].second == A[j].first && A[i].second == A[j].second)
                continue;
            if(A[i].second == A[j].first)
                isMax = 0;
        }
        if(isMax == 1)
            resultSize = add_to_array(Result, resultSize, A[i].second);
    }
    bubble_sort(Result, resultSize);
    return resultSize;
}
int find_min_elements(pair* A, int size, int* Result){
    int resultSize = 0;
    for(int i = 0; i < size; i++){
        int isMin = 1;
        for (int j = 0; j < size; j++){
            if (A[i].second == A[j].first && A[i].second == A[j].second)
                continue;
            if(A[i].first == A[j].second)
                isMin = 0;
        }
        if(isMin == 1)
            resultSize = add_to_array(Result, resultSize, A[i].first);
    }
    bubble_sort(Result, resultSize);
    return resultSize;
}

int cmp_int(int x, int y){
    if(x <= y)
        return 1;
    return 0;
}

int get_domain(pair* A, int size, int* Domain){
    int domainSize = 0;
    for(int i = 0; i < size; i++){
        domainSize = add_to_array(Domain, domainSize, A[i].first);
        domainSize = add_to_array(Domain, domainSize, A[i].second);
    }
    //qsort(Domain, domainSize, sizeof(int), cmp_int);
    bubble_sort(Domain, domainSize);
    return domainSize;
}

// Case 3:
int composition (pair* A, int sizeA, pair* B, int sizeB, pair* C){
    int sizeC = 0;
    for(int i = 0; i < sizeA; i++){
        for(int j = 0; j < sizeB; j++){
            if(A[i].second == B[j].first) {
                pair newR;
                newR.first = A[i].first;
                newR.second= B[j].second;
                sizeC = add_relation(C, sizeC, newR);
            }
        }
    }
    return sizeC;
}

int cmp (pair p1, pair p2) {
	if (p1.first == p2.first) return p1.second - p2.second;
	return p1.first - p2.first;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair* A){
    int size = 0;
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++){
        pair newPair;
        scanf("%d %d", &newPair.first, &newPair.second);
        size = add_relation(A, size, newPair);
    }
    return size;
}

void print_int_array(int *array, int n) {
	printf("%d\n", n);
	for (int i = 0; i < n; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int max_elements[MAX_REL_SIZE];
			int min_elements[MAX_REL_SIZE];
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size,
			   relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}


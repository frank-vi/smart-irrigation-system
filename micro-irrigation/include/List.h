#ifndef MY_LIST_H
#define MY_LIST_H

template<typename T>
class List {
public:
  List(int maxElems): maxElems(maxElems), elems(new T[maxElems]){
    nElems = 0;
  }

  bool add(T elem){
  	if (nElems < maxElems){
    	elems[nElems] = elem;
    	nElems++;
      return true;
  	}
    return false;
  }

  bool remove(T elem){
    for (int i = 0; i < nElems; i++){
      if (elems[i] == elem){
        for (int j = i+i; j < nElems; j++){
          elems[j-1] = elems[j];
        }
        nElems--;
        return true;
      }
    }
    return false;
  }

  T  get(int i){
	if (i < nElems){
      return elems[i];
    } else {
      return NULL;
    }
  }

  bool isEmpty(){
    return nElems == 0;
  }

  void empty(){
 	nElems = 0;
  }

  int size(){
  	return nElems;
  }

private:
  int nElems;
  int maxElems;
  T* elems;
};

#endif

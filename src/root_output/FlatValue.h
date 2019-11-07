#ifndef G4E_FLATVALUE_H
#define G4E_FLATVALUE_H

#include <TBranch.h>
#include <TTree.h>

/**
 * @file FlatValue
 */

class FlatValueBase {
public:
    void Clear() = 0;
    void BindToTree(TTree *tree) = 0;
};


template<class T>
class FlatValue: public FlatValueBase {
public:
    FlatValue(const std::string& name):
        fName(name)
    {
    }

    void Clear() {
        fValues.clear();
    }

    void BindToTree(TTree *tree){
        fBranch = tree->Branch(fName,  &fValues);
    }


    void AddValue(T value) {
        fValues.Add()
    }


private:

    std::string fName;
    std::vector<T> fValues;
    TBranch * fBranch;
};

#endif //G4E_FLATVALUE_H

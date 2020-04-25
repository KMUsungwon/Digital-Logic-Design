/*
 STUDENT NUMBER : 20171686
 NAME : 임성원
 DESCRIPTION : tabular method를 통해 얻는 PI(prime implicant)를 찾기 위한 알고리즘

 2020/4/25(토요일) : 코드에 대한 주석 추가했습니다.
 * */
#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;

//스트링이 오직 하나만 다른지 판별하기 위한 함수
bool compare(string a, string b) {
    int count = 0;
    for (int i = 0; i < a.length(); i++) {
        if (a[i] != b[i]) {
            count++;
        }
    }
    if (count == 1) return true;
    return false;
}

// 스트링이 오직 하나만 다른 문자의 위치를 '-'로 바꿔준다.
string getDiff(string a, string b) {
    for (int i = 0; i < a.length(); i++) {
        if (a[i] != b[i]) a[i] = '-';
    }
    return a;
}

//10진수로 입력받은 값을 비트에 맞게 2진수로 변환시켜주는 함수이다.
string DectoBin(int size, int x) {
    string bin;
    bin = bitset<100>(x).to_string();
    bin = bin.substr(100 - size);
    return bin;
}

//안묶인 PI의 인덱스를 찾아주기 위한 함수
int mark(int a, int indexSize, vector<int> idx) {
    int result = 0;

    for (int k = 0; k < indexSize; k++) {
        if (a == idx[k]) {
            result++;
            break;
        }
    }
    if (result == 0) {return a;}
    else return -1;
}

int main() {
    vector<int> minInt; //minterms을 10진수로 표현
    vector<string> minBin; //minterms을 2진수로 표현
    vector<string> newTable; //step을 한번 밟고난 값들을 임시로 저장하기 위한 벡터
    vector<string> primeImp; //최종 PI들의 값들이 저장되어 있는 벡터
    vector<string> primeImptemp; //최종 벡터의 전 단계 step의 값들이 저장되어 있는 벡터
    vector<string> tempTable; //더이상 줄일 수 없는 PI의 값들이 모여있는 벡터
    vector<int> idx; //몇번 째 값이 check가 되었는지 그 값의 인덱스들을 모아놓은 벡터
    vector<int> tempidx; //idx 벡터의 값들을 저장하기 위한 벡터

    //Initialise
    int nBits;
    int nMin;
    cout << "number of bits : ";
    cin >> nBits;

    cout << "number of minterms : ";
    cin >> nMin;

    cout << "Enter minterms : ";
    for (int i = 0; i < nMin; i++) {
        int x;
        cin >> x;
        minInt.push_back(x);
        minBin.push_back(DectoBin(nBits, x));
    }
    //print for input data
    cout << "My input data :" << '\n';
    for (int i = 0; i < nMin; i++) {
        cout << minInt[i] << " and bin type is.. " << minBin[i] << '\n';
    }

    //table 이라는 벡터에  2진수의 값들을 카피
    vector<string> table(minBin.size());
    for (int i = 0; i < minBin.size(); i++) {
        table[i] = minBin[i];
    };

    //setPrimeImp
    while (true) {
        tempidx.swap(idx);
        idx.clear();
        int count = 0; //count가 0이면 한자리만 다른 값들이 없다는 뜻이므로 반복문을 종료한다.

        for (int i = 0; i < table.size() - 1; ++i) {
            for (int j = i + 1; j < table.size(); ++j) {
                if (compare(table[i], table[j])) { //다른 자리가 한자리 밖에 없는지 비교
                    newTable.push_back(getDiff(table[i], table[j])); //getDiff 함수는 '-'를 붙혀서 그 스트링을 반환
                    idx.push_back(i); //그 값의 인덱스가 체크되었다고 표시하기 위함
                    idx.push_back(j); //그 값의 인덱스가 체크되었다고 표시하기 위함
                    ++count;
                }
            }
        }

        sort(idx.begin(), idx.end()); //인덱스 위치도 sorting 해서 중복 제거 0 1 1 2 -> 0 1 2 이런식으로
        idx.erase(unique(idx.begin(), idx.end()), idx.end());




        tempTable.assign(table.begin(), table.end()); //테이블을 이제 비우기 전에 최종 단계를 거친 pi값들을 tempTable에 저장
        table.clear(); //table은 newTable의 값들을 저장하기 위해 비우기
        table.resize(newTable.size()); // table의 크기를 재정의함
        //table에 newTable의 값들을 저장
        for (int i = 0; i < newTable.size(); i++) {
            table[i] = newTable[i];
        }
        newTable.clear(); //newTable도 비워줌. 다시 for문을 돌아야 하기 때문에


        //한번 step을 밟고 난 후에 값들 중에서 중복된 것이 있으면 지워주기 위한 코드
        sort(table.begin(), table.end());
        table.erase(unique(table.begin(), table.end()), table.end());



        if (count == 0) {
            cout << "\n--------------------" << '\n';
            //최종 PI를 담는 primeImp에 tempTable, 즉 더 이상 줄일 수 없는 값들을 넣어준다.
            for(int i=0; i<tempTable.size(); i++) {
                primeImp.push_back(tempTable[i]);
            }
            //전 단계에서 체크되지 않은 값들을  포함시키지 않은 완성되지 않은 PI의 값을 출력
            cout << "NOT COMPLETE PI : ";
            for(int i=0; i<primeImp.size(); i++) {
                cout << primeImp[i] << " ";
            }
            cout << '\n';

            //최종보다 한 단계 전의 벡터를 만듦
            for(int i=0; i< minBin.size()-1; ++i) {
                for(int j= i+1; j<minBin.size(); j++) {
                    if(compare(minBin[i],minBin[j])) {
                        primeImptemp.push_back(getDiff(minBin[i],minBin[j]));
                    }
                }
            }
            sort(primeImptemp.begin(), primeImptemp.end());
            // 최종의 전 단계 벡터 출력
            cout << "previous last step vector : ";
            for(int temp=0; temp<primeImptemp.size(); ++temp) {
                cout << primeImptemp[temp] << ' ';
            }
            //최종 전 단계의 값들이 저장되어 있는 벡터에서 체크가 된 값들의 인덱스를 출력
            cout << "\nend checked index are : ";
            for(int i=0; i<tempidx.size(); i++) {
                cout << tempidx[i] << " ";
            }

            //체크되지 않은 인덱스를 찾아 그 값을 최종 PI 벡터에 넣어준다.
            for (int i = 0; i < primeImptemp.size(); i++) {
                int result = mark(i, tempidx.size(), tempidx);
                if (result != -1)
                    primeImp.push_back(primeImptemp[result]);
            }
            break;
        }

    }

    //최종 PI가 담겨있는 벡터들의 값들을 출력
    cout << "\nSo, all of PI : ";
    for(int i=0; i<primeImp.size(); i++) {
        cout << primeImp[i] << " ";
    }
    cout << '\n';
    return 0;
}


#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>


#define Red 0
#define Black 1
//블랙과 레드 값을 0과 1로 설정
#define external 1;
#define internal 0;
//내부노드를 0 외부노드를 1로 설정
using namespace std;

bool duplication = 0;
//중복된 회원넘버를 확인하는 변수

struct Vip {
    int membernumber;
    int totalPrice;
};
//VipList에 저장 될 회원번호와 총금액

struct VipHigh {
    bool operator()(const Vip& a, const Vip& b)const
    {
        return a.totalPrice> b.totalPrice;
    }
};
//VipList에서 결제 금액 순으로 정렬해줄 오퍼레이션

vector<Vip> VipList;
//결제 금액이 높은, 상위우수 회원의 결제금액을 구하기 위한 벡터 생성


ofstream fout("output.txt");
//출력파일을 오픈

class RBNode {
public:
    int color = Black; //기본값은 Black
    int ternal = external; //기본값은 external
    RBNode* left = nullptr;
    RBNode* right = nullptr;
    RBNode* parent = nullptr;
    //자식,부모 노드들의 기본값은 null
    int memberNumber = 0;
    //회원번호를 key로 회원정보들을 입력 받음.
    //
    //회원번호 , 회원이름, 연락처, 주소의 x좌표, 주소의 y좌표, 회원등급, 총 결제금액
    //
    struct PList {
        int paymentDay;
        char productName[20];
        int paymentPrice;
    };
    //회원이 구입한 내역을 구조체로 생성
    
    struct information {
        char memberName[20];
        char phoneNumber[11];
        int addressX;
        int addressY;
        int memberLevel = 0;
        int limitPayment = 0;
        //물품 구매 승인시 필요한 한도금액
        int totalPrice = 0;
        int productNum = 0;
        //추가될 물건의 갯수
        vector<PList> paymentList;
        //한 회원이 구입한 결제 내역을 벡터로 생성
    };
    
    
    
    information memberInfo;
    //회원정보 구조체 생성
    
    struct paymentDayRecent {
        bool operator()(const PList& a, const PList& b)const
        {
            return a.paymentDay > b.paymentDay;
        }
    };
    //결제내역 정렬시 사용할 비교 오퍼레이션
    
    void setinformation(char memberName[], char phoneNumber[], int addressX, int addressY, int level, int totalPrice) {
        strcpy(memberInfo.memberName, memberName);
        strcpy(memberInfo.phoneNumber, phoneNumber);
        memberInfo.addressX = addressX;
        memberInfo.addressY = addressY;
        memberInfo.memberLevel = level;
        memberInfo.totalPrice = totalPrice;
        memberInfo.limitPayment = 3000000 * (pow(10, level));//등급에 따른 한도 설정
    };
    //노드의 구조체에 회원정보를 입력하는 함수 생성
    
    RBNode(bool black = true, bool ex_ternal = true) {
        color = black;
        ternal = ex_ternal;
    }
    //노드 생성시 색과, 터널 설정
    
    RBNode* getleft() { return left; }
    RBNode* getright() { return right; }
    RBNode* getparent() { return parent; }
    //왼쪽, 오른쪽 자식과 부모를 반환하는 함수
    int getcolor() { return color; }
    //노드의 색을 반환하는 함수
    void setmemberNumber(int v) { memberNumber = v; }
    //노드의 값을 저장하는 함수
    int getmemberNumber() { return memberNumber; }
    //노드의 값을 반환하는 함수
    void _red() { color = Red; }
    void _black() { color = Black; }
    //컬러를 바꿔주는 함수
    void _internal() { ternal = internal; }
    void _external() { ternal = external; }
    //external인지 internal인지 정하는 함수
    
    void assignleft(RBNode *L) { left = L; if (L) L->parent = this; }
    void assignright(RBNode *R) { right = R; if (R) R->parent = this; }
    void assignparent(RBNode *P) { parent = P; }
    //왼쪽, 오른쪽 자식, 부모 할당 함수
    
    
    void recolor() {
        color = !(color);
    }
    //색을 바꿔주는 함수
    void changecolor(RBNode *n1, RBNode *n2) {
        int temp = n2->getcolor();
        n2->color = n1->color;
        n1->color = temp;
    }
    //색을 교환해주는 함수
    
    bool check_black() {
        //해당 노드가 블랙이면 1 출력하는 함수
        if (color) {
            return true;
            //B노드이면 1
        }
        return false;
        //아니면 0
    }
    
    bool check_red() {
        //해당 노드가 레드이면 1을 출력하는 함수
        return !check_black();
        //B체크 반대로 값 반환
    }
    
    bool check_ex() {
        //해당 노드가 외부 노드이면 1을 출력하는 함수
        if (ternal) {
            return true;
            //외부노드이면 true 반환
        }
        return false;
        //내부노드이면 false 반환
    }
    
    bool check_in() {
        //해당 노드가 내부 노드이면 1을 출력하는 함수
        return !check_ex();
        //ex 체크 반대로 값 반환
    }
    
    RBNode *sibling();
    //삼촌을 반환하는 함수
    
    
    
};

RBNode *RBNode::sibling() {
    //삼촌을 반환하는 함수
    if (parent == NULL)
        return NULL;
    //부모가 없으면 형제노드가 없다
    if (parent->left == this) {
        //자신이 부모의 왼쪽 자식이면 형제노드로 오른쪽자식 반환
        return parent->right;
    }
    
    return parent->left;
    //자신이 부모의 오른쪽 자식이면 현제노드로 왼쪽자식 반환
    
}

class RBTree {
public:
    
    RBNode *getroot() { return root; }
    //루트를 반환하는 함수
    void setroot(RBNode* P) { root = P; }
    //루트를 설정하는 함수
    void RBinsert(int memberNumber, char memberName[], char phoneNumber[], int addressX, int addressY, int level, int totalPrice);
    //void RBsearch(int i);
    void CheckRR(RBNode* n);
    //더블레드를 확인 하는 함수
    RBNode *_search(int i);
    //노드 위치 찾기
    bool checkNode(int i);
    //노드가 있는지 확인 하는 함수
    void recoloring(RBNode *n);
    //리컬러링
    void restructuring(RBNode *n);
    //리스트럭쳐링
    
    void print_memberInfo(RBNode *n);
    //노드의 멤버 정보를 출력하는 함수
    
    void checkPaymentList(int memberNumber, int many);
    //해당회원의 물품 구매내역을 출력하는 함수
    
    void confirmPayment(int memberNumber, int day, char productName[], int paymentPrice);
    //결제 승인 함수, 승인이면 목록에 추가
    
    RBNode *root = nullptr;
    //루트노드의 기본값은 null
    void print(RBNode *, int *ar, char *RBar, int position);
    //레드블랙 트리 인쇄 디버깅용
    int getdepth(RBNode *n);
    //노드의 깊이를 구하는 함수
    
};




RBNode *RBTree::_search(int i) {
    //key값을 입력하면, 노드의 key값을 비교하여
    //입력할 노드의 위치를 반환 해주는 함수
    RBNode *current = getroot();
    //루트 노드에서부터 탐색을 시작함
    
    while (current != nullptr && current->getmemberNumber() != i && current->check_in()) {
        //루트가 있고, 중복된 값이아니고 , 현재 위치가 내부노드일때
        if (i < current->getmemberNumber()) {
            current = current->getleft();
            //입력하는 값이 현재 위치의 값보다 작을때 왼쪽 자식 노드로 내려감
        }
        else if (i > current->getmemberNumber()) {
            current = current->getright();
        }
        //작으면 오른쪽 자식 노드로 내려감
    }
    return current;
    //현재 위치 반환
}

bool RBTree::checkNode(int i) {
    //노드가 존재하는지 확인 하는 함수
    if (getroot()) {
        RBNode *current = getroot();
        while (current->check_in()) {
            if (i < current->getmemberNumber()) {
                current = current->getleft();
            }
            else if (i > current->getmemberNumber()) {
                current = current->getright();
            }
            else if (i == current->getmemberNumber()) {
                return true;
                //현재 위치의 값이 검색하는 값과 같을때 true 반환
            }
        }
    }
    else {
        //루트가 없으면 트리는 존재 하지 않음
        return false;
    }
    return false;
}


void RBTree::recoloring(RBNode *n) {
    // 조부모가 루트일 경우 아빠와 삼촌 노드의 색만 변환
    // 조부모가 루트가 아닐 경우, 조부모, 아빠, 삼촌 노드 모두 색 변환
    RBNode *parent_node;
    RBNode *pparent_node;
    
    parent_node = n->parent;
    pparent_node = n->parent->parent;
    
    if (pparent_node == getroot()) {
        //루트노드일 경우, 루트 노드의 색을 변환하지 않음
        pparent_node->left->recolor();
        pparent_node->right->recolor();
        //조부모의 자식들 색변환
    }
    else {
        //루트노드가 아닐경우 조부모의 왼쪽,오른쪽자식과 , 조부모 노드 색을 바꿈
        pparent_node->recolor();
        pparent_node->left->recolor();
        pparent_node->right->recolor();
    }
    
}

void RBTree::restructuring(RBNode *n) {
    //리스트럭처링 함수
    RBNode *parent_node;
    RBNode *pparent_node;
    RBNode *ppparent_node;
    RBNode *top_node = nullptr;
    RBNode *temp_node;
    RBNode *temp_node2;
    //임시 노드
    
    parent_node = n->getparent();
    pparent_node = n->getparent()->getparent();
    ppparent_node = n->getparent()->getparent()->getparent();
    
    //구조를 바꿀때 4가지 경우
    //부모가 왼쪽 자식일때
    if (pparent_node->getleft() == parent_node) {
        if (parent_node->getleft() == n) {
            //자신이 왼쪽 자식일때
            //      pp     p
            //    p    > n   pp
            //  n   t       t
            // 위 모양처럼 리스트럭처링이 일어난다.
            // p노드가 상위로 올라기기 전에 오른쪽 자식노드를 임시노드로 저장후 조부모노드 왼쪽 자식에 연결
            temp_node = parent_node->getright();
            //부모 노드에 붙어있는 오른쪽 자식을 떼어놓기
            
            top_node = parent_node;
            //맨위 노드
            parent_node->assignparent(NULL);
            //맨위 노드가 된 노드의 부모는 초기화 시켜준 후 나중에 이음
            parent_node->assignright(pparent_node);
            //부모의 오른쪽자식은 조부모노드
            pparent_node->assignparent(parent_node);
            //조부모 노드의 부모는 부모노드
            
            pparent_node->assignleft(temp_node);
            //조부모 노드의 왼쪽 자식은 부모노드의 오른쪽 자식
            pparent_node->getleft()->assignparent(pparent_node);
            //조부모노드의 왼쪽자식에 부모 경로를 설정
            parent_node->changecolor(pparent_node, parent_node);
            //부모와 부모의부모 노드 색 교환
            
        }
        else if (parent_node->getright() == n) {
            //자신이 오른쪽 자식일때
            //      pp          n
            //    p      >   p     pp
            //      n         t1 t2
            //    t1  t2
            // 위 모양처럼 리스터럭처링이 이렁난다
            // n노드가 가진 오른쪽자식 t1은 p노드의 오른쪽자식으로, n노드의 왼쪽자식 t2는 pp노드의 왼쪽자식으로 연결
            temp_node = n->getleft();
            temp_node2 = n->getright();
            //n의 자식노드들을 임시노드에 저장
            top_node = n;
            //n을 상위 노드와 연결하기 위해서 임시노드로 저장
            
            n->assignparent(NULL);
            //n의 부모경로를 없앰, 추후 루트가 아닐때 이어줄 예정
            n->assignleft(parent_node);
            parent_node->assignparent(n);
            //n의 왼쪽 자식노드에 부모노드를 연결, 부모노드의 부모를 n으로 설정
            n->assignright(pparent_node);
            pparent_node->assignparent(n);
            //n의 오른쪽 자식노드에 조부모 노드 연결, 조부모의 부모를 n으로 설정
            parent_node->assignright(temp_node);
            parent_node->getright()->assignparent(parent_node);
            pparent_node->assignleft(temp_node2);
            pparent_node->getleft()->assignparent(pparent_node);
            //임시노드에 저장한 n노드의 자식노드들을 부모와 조부모 노드에 연결, 부모경로도 연결
            n->changecolor(pparent_node, n);
            //n과 조부모노드 색 변환
        }
    }
    else if (pparent_node->getright() == parent_node) {
        //부모가 오른쪽 자식일때
        if (parent_node->getright() == n) {
            //자신이 오른쪽 자식일때
            //  pp         p
            //    p  >  pp   n
            //  t1  n     t1
            // 위 모양처럼 리스트럭처링이 일어난다.
            // p노드의 왼쪽자식 t1을 임시 노드로 저장후, pp 노드의 오른쪽 자식으로 연결
            temp_node = parent_node->getleft();
            //부모의 왼쪽 자식 노드는 임시노드로 저장
            top_node = parent_node;
            //부모노드는 상위 임시노드로 저장
            parent_node->assignparent(NULL);
            //부모 경로를 제거, 추후 루트가 아닐경우 연결
            parent_node->assignleft(pparent_node);
            //부모노드의 왼쪽자식을 조부모 노드로 연결
            pparent_node->assignparent(parent_node);
            //조부모 노드의 부모를 부모노드로 연결
            pparent_node->assignright(temp_node);
            pparent_node->getright()->assignparent(pparent_node);
            //조부모 노드의 오른쪽 자식을 임시 노드로, 조부모와 조부모의 오른쪽 자식 연결
            parent_node->changecolor(parent_node, pparent_node);
            //부모노드와 조부모 노드 색변환
            
        }
        else if (parent_node->getleft() == n) {
            //자신이 왼쪽 자식일때
            //  pp          n
            //     p   > pp    p
            //   n		  t1 t2
            // t1  t2
            // 위 모양처럼 리스트럭처링이 일어난다.
            // n노드가 가진 왼쪽 노드 t1 , 오른쪽 노드 t2를 임시 노드에 저장 후
            // pp 노드의 오른쪽 자식에 t1 , p노드의 오른쪽 자식에 t2를 연결
            
            temp_node = n->getleft();
            temp_node2 = n->getright();
            //n의 자식들을 임시 노드에 저장
            top_node = n;
            //n을 상위 임시노드로 저장
            n->assignparent(NULL);
            //n의 부모경로 제거
            n->assignleft(pparent_node);
            //n의 왼쪽 자식를 조부모 노드로 설정
            pparent_node->assignparent(n);
            //조부모의 부모 경로를 n으로 설정
            
            n->assignright(parent_node);
            //n의 오른쪽 자식을 부모노드로 설정
            parent_node->assignparent(n);
            //부모 노드의 부모를 n으로 설정
            
            pparent_node->assignright(temp_node);
            //떼어놓은 n노드의 왼쪽자식을 조부모노드 오른쪽 자식으로 연결
            pparent_node->getright()->assignparent(pparent_node);
            //조부모의 오른쪽 자식에게 부모노드 경로 설정
            
            parent_node->assignleft(temp_node2);
            //부모 노드의 왼쪽 자식을 n의 오른쪽 자식으로 설정
            parent_node->getleft()->assignparent(parent_node);
            //부모노드의 왼쪽 자식에게 부모노드 경로 설정
            
            n->changecolor(n, pparent_node);
            //n과 조부모 노드 색변환
        }
    }
    
    
    if (ppparent_node) {
        //리스트럭처가 끝난 꼭대기의 노드가 루트가 아닐경우에
        //그 상위노드였던 노드와 연결하는 작업
        if (ppparent_node->getleft() == pparent_node) {
            ppparent_node->assignleft(top_node);
            ppparent_node->getleft()->assignparent(ppparent_node);
        }
        else {
            ppparent_node->assignright(top_node);
            ppparent_node->getright()->assignparent(ppparent_node);
        }
    }
    else {
        //3번째 부모가 없을경우 최상 노드는 루트로 세팅
        //이미 상위 노드의 부모 경로는 제거해놓음
        setroot(top_node);
        
    }
    
}

void RBTree::print_memberInfo(RBNode *n) {
    //해당노드의 정보 출력
    //이름,번호,주소x,y,등급,결제금액
    //printf("%s %s %d %d %d %d ",n->memberInfo.memberName,n->memberInfo.phoneNumber,n->memberInfo.addressX,n->memberInfo.addressY,n->memberInfo.memberLevel,n->memberInfo.totalPrice);
    fout << n->memberInfo.memberName << " " << n->memberInfo.phoneNumber << " " << n->memberInfo.addressX << " " << n->memberInfo.addressY << " " << n->memberInfo.memberLevel << " " << n->memberInfo.totalPrice << " ";
}


void RBTree::CheckRR(RBNode *n) {
    //더블 레드 확인 함수
    RBNode *parent_node;
    RBNode *pparent_node;
    parent_node = n->getparent();
    pparent_node = n->getparent()->getparent();
    
    if (parent_node && parent_node->check_red() && n->check_red()) {
        //자기자신과 부모가 레드이면 더블레드 문제 발생
        if (parent_node->sibling()->check_black()) {
            //부모의형제가 블랙이면 리스트럭쳐
            restructuring(n);
        }
        else if (parent_node->sibling()->check_red()) {
            //부모의형제가 레드이면 리컬러링
            recoloring(n);
            if (pparent_node != getroot())
                //루트까지 더블레드 확인
                //부모의 부모노드가 루트일경우 재귀함수 호출하지 않음
                CheckRR(pparent_node);
        }
    }
}

void RBTree::RBinsert(int memberNumber, char memberName[], char phoneNumber[], int addressX, int addressY, int level, int totalPrice) {
    //레드블렉트리에 노드를 추가해주는 함수
    //루트가 없는경우 루트노드로 추가해주고,
    //루트가 있는 경우, key가 크면 오른쪽자식, 작으면 왼쪽자식 방향으로 내려가며
    //key 자리를 찾아 노드를 저장함
    RBNode *n;
    
    n = _search(memberNumber);
    //key인 회원 번호로, 신규회원 정보가 추가될 노드위치를 가져옴
    
    
    if (RBTree::getroot() == nullptr) {
        n = new RBNode(true, false);
        //black노드, internal노드
        n->setmemberNumber(memberNumber);
        n->setinformation(memberName, phoneNumber, addressX, addressY, level, totalPrice);
        //회원번호로 생성된 노드에 회원정보 추가
        //root 노드 추가
        n->assignleft(new RBNode);
        n->assignright(new RBNode);
        //external 자식노드 추가, 기본값은 블랙,익스터널
        n->memberInfo.paymentList.clear();
        //벡터리스트 초기화
        
        Vip tempVip = { memberNumber, totalPrice };
        //상위 회원 목록에 넣을 구조체 설정
        VipList.push_back(tempVip);
        //VipList에 총금액 추가
        //sort(VipList.begin(), VipList.end(), VipHigh());
        //VipList 총결제 금액으로 정렬
        
        setroot(n);
        //처음 추가되는 노드는 루트로 설정
        duplication = 0;
        //루트가 추가되면 중복 확인 인자는 0 설정
    }
    else if (n->check_ex()) {
        
        n->_internal();
        n->_red();
        //해당 노드를 내부, 레드로 변경
        n->setmemberNumber(memberNumber);
        n->setinformation(memberName, phoneNumber, addressX, addressY, level, totalPrice);
        //회원번호로 생성된 노드에 회원정보 추가
        n->memberInfo.paymentList.clear();
        //벡터리스트 초기화
        n->assignleft(new RBNode);
        n->assignright(new RBNode);
        //자식 노드들을 블랙, 익스터널로 삽입
        
        Vip tempVip = { memberNumber, totalPrice };
        //상위 회원 목록에 넣을 구조체 설정
        VipList.push_back(tempVip);
        //VipList에 총금액 추가
        //sort(VipList.begin(), VipList.end(), VipHigh());
        //VipList 총결제 금액으로 정렬
        
        CheckRR(n);
        //더블레드 확인하여 트리 수정
        duplication = 0;
    }
    else {
        duplication = 1;
        //노드가 추가되지 못했으면 중복이므로 중복확인 변수 변경
    }
    
    
}

void RBTree::print(RBNode *n, int *ar, char *RBar, int position) {
    //배열에 레드블랙 트리 생성
    //초기 레드블랙 트리 생성 디버깅 함수
    if (n == NULL) {
        return;
    }
    else {
        ar[position] = n->getmemberNumber();
        if (n->check_black()) {
            RBar[position] = 'B';
        }
        else {
            RBar[position] = 'R';
        }
        print(n->left, ar, RBar, position * 2);
        print(n->right, ar, RBar, position * 2 + 1);
    }
}

void tree_print(int *ar, char *RBar, int size) {
    //트리모양 출력 디버깅을 위해 필요
    //초기 레드블랙 트리 생성 디버깅 함수
    int k = 1;
    for (int i = 1; i < size; i++) {
        if (RBar[i] != 0) {
            printf("\t%d%c", ar[i], RBar[i]);
        }
        else {
            printf("\t");
        }
        if (i == pow(2, k) - 1) {
            cout << endl;
            k++;
        }
    }
}

void RBTree::checkPaymentList(int memberNumber, int many) {
    //해당 회원의 구매내역을 출력해주는 함수
    RBNode *n;
    if (checkNode(memberNumber)) {
        n = _search(memberNumber);
        //노드 위치
        vector<RBNode::PList>::iterator iter;
        //노드의 벡터 이터레이터 설정
        
        sort(n->memberInfo.paymentList.begin(), n->memberInfo.paymentList.begin() + n->memberInfo.productNum, RBNode::paymentDayRecent());
        //결제날짜로 내림차순 정렬
        
        if (n->memberInfo.paymentList.end() - n->memberInfo.paymentList.begin()) {
            //처음과 끝이 같다면, 목록이 존재하지 않음 출력 0
            
            if (many < n->memberInfo.productNum) {
                //물품구매 승인을 받을때마다 증가하는 productNum변수로 회원의 구매내역 수를 판별
                //회원번호와 출력개수를 입력 받았을때, 출력개수가 구매내역 수보다 작을 경우a
                //출력개수 만큼 출력하고 출력개수가 구매내역 수보다 클 경우 구매내역 수만큼만 출력
                for (iter = n->memberInfo.paymentList.begin(); iter != n->memberInfo.paymentList.begin() + many; iter++) {
                    //백터 시작지점 부터 입력한 갯수 만큼만 출력
                    
                    //printf("%d %s %d\n", iter->paymentDay,iter->productName,iter->paymentPrice);
                    fout << iter->paymentDay << " " << iter->productName << " " << iter->paymentPrice << "\n";
                    
                }
            }
            else {
                for (iter = n->memberInfo.paymentList.begin(); iter != n->memberInfo.paymentList.begin() + (n->memberInfo.productNum); iter++) {
                    //백터 시작지점 부터 입력(구매)한 갯수 만큼만 출력
                    
                    //printf("%d %s %d\n", iter->paymentDay,iter->productName,iter->paymentPrice);
                    fout << iter->paymentDay << " " << iter->productName << " " << iter->paymentPrice << "\n";
                    
                }
            }
        }
        else {
            //printf("0\n");
            fout << "0\n";
            //목록이 없으면 0출력
        }
    }
    else {
        //printf("Not found!\n");
        fout << "Not found!\n";
        //회원이 존재하지 않음
    }
}

void RBTree::confirmPayment(int memberNumber, int day, char productName[], int paymentPrice) {
    //물품 구매시 해당 회원의 한계 금액을 초과하지 않으면, 구매를 승인하고
    //물품 구매정보를 회원정보의 결제내역리스트에 추가, 회원정보의 총 결제 금액에
    //물품 구매금액을 추가함. 상위 결제 금액 회원 리스트를 매번 갱신해줌
    RBNode *n;
    //구조체 선언
    RBNode::PList templist;
    //구매내역 구조체 임시로 생성
    
    
    if (checkNode(memberNumber)) {
        n = _search(memberNumber);
        if (n->memberInfo.limitPayment > (paymentPrice + n->memberInfo.totalPrice)) {
            //한도보다 구매할 비용이 작으면 승인
            n->memberInfo.totalPrice += paymentPrice;
            //구매한 비용 총결제 금액에 추가
            
            vector<Vip>::iterator iter;
            //노드의 벡터 이터레이터 설정
            
            for (iter = VipList.begin(); iter != VipList.end(); iter++) {
                //VipList에 회원번호와 맞는 리스트에 결제금액 추가
                if (iter->membernumber == memberNumber) {
                    iter->totalPrice += paymentPrice;
                    
                }
            }
            
            templist.paymentDay = day;
            strcpy(templist.productName, productName);
            templist.paymentPrice = paymentPrice;
            //벡터에 집어넣을 임시 구조체에 값 대입
            
            n->memberInfo.paymentList.push_back(templist);
            //벡터에 결재내역을 추가
            //printf("%d %d\n",getdepth(n),1);
            fout << getdepth(n) << " " << 1 << "\n";
            //결제내역에 삽입 되었을때 깊이와 1출력
            n->memberInfo.productNum++;
            
        }
        else {
            //printf("%d %d\n",getdepth(n),0);
            fout << getdepth(n) << " " << 0 << "\n";
            //결제내역에 삽입 되었을때 깊이와 0출력
        }
    }
    else {
        //printf("Not found!\n");
        fout << "Not found!\n";
        //노드가 없을 경우
    }
    
};
//결제 승인 함수, 승인이면 목록에 추가

int RBTree::getdepth(RBNode *n) {
    RBNode *current = getroot();
    //루트 노드에서부터 탐색을 시작함
    int depth = 0;
    int i = n->getmemberNumber();
    while (current != nullptr && current->getmemberNumber() != i && current->check_in()) {
        //루트가 있고, 루트값이 찾는 값이 아니며, 현재 위치가 내부노드일때
        if (i < current->getmemberNumber()) {
            depth++;
            current = current->getleft();
        }
        //입력하는 값이 현재 위치의 값보다 작을때 왼쪽 자식 노드로 내려감
        else if (i > current->getmemberNumber()) {
            depth++;
            current = current->getright();
        }
        else if (i == current->getmemberNumber()) {
            return depth;
        }
        //작으면 오른쪽 자식 노드로 내려감
    }
    return depth;
}
//노드의 깊이를 구하는 함수



int main() {
    RBTree RedBlackTree;
    
    int memberNumber = 0;
    char phoneNumber[11] = { 0, };
    int addressX = 0;
    int addressY = 0;
    int level = 0;
    int totalPrice = 0;
    char memberName[20] = { 0, };
    char switchO = 0;
    //변수 초기화
    
    printf("입력파일의 이름을 입력하세요 :");
    char fileName[50];
    char fileName2[50];
    cin >> fileName;
    
    
    ifstream fin(fileName);
    
    fin >> fileName2;
    ifstream fin2(fileName2);
    
    
    while (fin2 >> memberNumber >> memberName >> phoneNumber >> addressX >> addressY >> level >> totalPrice) {
        RedBlackTree.RBinsert(memberNumber, memberName, phoneNumber, addressX, addressY, level, totalPrice);
        //데이터 입력
    }
    fin2.close();
    
    while (switchO != 'Q') {
        //Q를 입력받으면 프로그램 종료
        while (fin >> switchO) {
            
            switch (switchO) {
                case 'I':
                {
                    //신규 회원 가입
                    fin >> memberNumber >> memberName >> phoneNumber >> addressX >> addressY;
                    
                    RedBlackTree.RBinsert(memberNumber, memberName, phoneNumber, addressX, addressY, 0, 0);
                    if (duplication == 0) {
                        //printf("%d %d\n",RedBlackTree.getdepth(RedBlackTree._search(memberNumber)),1);
                        fout << RedBlackTree.getdepth(RedBlackTree._search(memberNumber)) << " " << 1 << "\n";
                    }
                    else if (duplication == 1) {
                        //printf("%d %d\n",RedBlackTree.getdepth(RedBlackTree._search(memberNumber)),0);
                        fout << RedBlackTree.getdepth(RedBlackTree._search(memberNumber)) << " " << 0 << "\n";
                        duplication = 0;
                    }
                    break;
                }
                    
                case 'P':
                {
                    int number = 0;
                    //원하는 회원 정보 출력
                    fin >> number;
                    if (RedBlackTree.checkNode(number)) {
                        RedBlackTree.print_memberInfo(RedBlackTree._search(number));
                        //printf("%d\n",RedBlackTree.getdepth(RedBlackTree._search(number)));
                        fout << RedBlackTree.getdepth(RedBlackTree._search(number)) << "\n";
                    }
                    else {
                        //printf("Not found!\n");
                        fout << "Not found!\n";
                    }
                    break;
                }
                case 'A':
                {
                    int number = 0, day = 0, payPrice = 0;
                    char productName[20] = { 0, };
                    //해당 회원번호의 회원이 제품구매
                    fin >> number >> day >> productName >> payPrice;
                    RedBlackTree.confirmPayment(number, day, productName, payPrice);
                    break;
                }
                case 'R':
                {
                    //해당 회원번호의 회원이 구입한 내역 출력
                    int number, many;
                    fin >> number >> many;
                    RedBlackTree.checkPaymentList(number, many);
                    break;
                }
                case 'F':
                {
                    //결제 금액이 높은 상위 3명의 결제 금액 출력
                    vector<Vip>::iterator iter;
                    //노드의 벡터 이터레이터 설정
                    
                    sort(VipList.begin(), VipList.end(), VipHigh());
                    //상위 결제금액 리스트를 정렬함
                    
                    
                    for (iter = VipList.begin(); iter != VipList.begin() + 3; iter++) {
                        //상위 결제금액 리스트에서 3개의 명단 출력
                        //printf("%d ",iter->totalPrice);
                        fout << iter->totalPrice << " ";
                    }
                    cout << endl;
                    fout << "\n";
                    break;
                }
                    
            }
            
            
            
        }
    }
    fin.close();
    //파일입력 해제
    fout.close();
    //파일출력 헤제
    return 0;
}



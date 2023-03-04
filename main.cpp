#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 800;

//the window we'll be redering to
SDL_Window* gWindow = NULL;

//the renderer we'll be using to render art
SDL_Renderer* gRenderer = NULL;

//initialize SDL
bool init();

//closing SDL
bool close();

//drawcircle
void drawCircle(SDL_Renderer* renderer, int x, int y, int r);

bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else {
        
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else {
            //create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s/n", SDL_GetError());
                success = false;
            }
            else {
                //Set color for renderer
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            }
        }
    }
    return success;
}

bool close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    SDL_Quit();
    return true;
}

void drawCircle(SDL_Renderer* renderer, int x, int y, int r) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (float i = -r/sqrt(2); i <= r/sqrt(2); i += 0.01) {
        SDL_RenderDrawPoint(renderer, x + i, y + sqrt(r*r - i*i));
        SDL_RenderDrawPoint(renderer, x + i, y - sqrt(r*r - i*i));
    }
    for (float i = -r/sqrt(2); i <= r/sqrt(2); i += 0.2) {
        SDL_RenderDrawPoint(renderer, x + sqrt(r*r - i*i), y + i);
        SDL_RenderDrawPoint(renderer, x - sqrt(r*r - i*i), y + i);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

//random 1 số từ l đến h
long long Rand(int l, int h) {
    return (long long)rand() % (h - l) + l;
}

//Khai báo các tham số, quần thể, dữ liệu liên quan
const double xsditruyen = 0.7;
const double xsbiendi = 0.1;
const double r1 = 300; //bán kính trường hợp nông thôn
const double r2 = 50; //bán kính trường hợp đô thị
const double r = r2; //bán kính của trạm phát(tính theo dam - đề ca mét)

const int r_area1 = 1500000;//độ dài cạnh phạm vi xét hình vuông khu vực nông thôn
const int r_area2 = 200000;//độ dài cạnh phạm vi xét hình vuông khu vực đô thị
const int r_area = r_area2;//đơn vị cm

const int quanthe = 50;//số lượng quần thể ban đầu
int number = 50;//số lượt di truyền
int numberrandom = 5000; //số lượt random dùng cho lời giải random

vector<pair<double, double>> v;//tập quần thể
vector<pair<double, double>> house;//tập hộ gđ
vector<pair<double, double>> cathemax;//tập chứa các cá thể max ở từng vòng đời

string path = "Ditruyen2.txt";

//tính độ thích nghi của từng cá thể {x, y}
int f(vector<pair<double, double>> house, pair<double, double> p) {
  int count = 0;
  for(auto c : house) {
    double x = p.first, y = p.second, z = c.first, t = c.second;
    x = abs(x - z);
    y = abs(y - t);
    if((x * x + y * y) <= r * r) ++count;
  }
  return count;
}

//hàm sort cho quần thể
double sort_v(pair<double, double> a, pair<double, double> b) {
    return f(house, a) < f(house, b);
}

//tìm cá thể có độ thích nghi lớn nhất.
pair<double, double> vitri(vector<pair<double, double>> house, vector<pair<double, double>> v) {
  int mx = 0;
  pair<double, double> ans = v[0];
  for(auto c : v) {
    if(mx < f(house, c)) {
      mx = f(house, c);
      ans = c;
    }
  }
  return ans;
}

//tạo mảng chứa xác suất tích lũy của quần thế
vector<double> xstichluy(vector<pair<double, double>> house, vector<pair<double, double>> v) {
    vector<double> ans; //tập hợp chứa xác suất tích luỹ
    vector<double> abc; //tập hợp chứa xác suất tích luỹ chưa chuẩn hoá
    for(int i = 0; i < v.size(); ++i) {
        if(i == 0) abc.push_back(f(house, v[i]));
        else abc.push_back(abc[abc.size() - 1] + f(house, v[i]));
    }
    if(abc[abc.size() - 1] == 0) return {0};
    for(auto c : abc) {
        c = c / abc[abc.size() - 1];
        ans.push_back(c);
    }
    return ans;
}

//tạo các hộ gia đình
//void houses() {
//  //tọa độ điểm tập trung
//    cout << "Toạ độ các điểm tập trung:" << endl;
//  double arrx[5], arry[5];
//  for(int i = 0; i < 5; ++i) {
//    arrx[i] = Rand(1, r_area);
//    arry[i] = Rand(1, r_area);
//    arrx[i] /= 1000;
//    arry[i] /= 1000;
//      cout << arrx[i] << " " << arry[i] << endl;
//  }
//    cout << endl;
//
//  int n = 50000;
//  //kéo các điểm gần lại các điểm tập trung
//  cout << endl << "Toa do cua cac ho gia dinh la:" << endl << endl;
//  for(int i = 0; i < n; ++i) {
//    double ratio = rand() % 48  + 1;
//    double x = Rand(1, r_area) + 1;
//    double y = Rand(1, r_area) + 1;
//    x /= 1000; y /= 1000;
//
//    x = x + (arrx[i % 5] - x) * ratio / 50;
//    y = y + (arry[i % 5] - y) * ratio / 50;
//    house.push_back({x, y});
//  }
//}

void houses() {
    ifstream fin(path);
    int houses_number;
    fin >> houses_number;
    double x;
    double y;
    for (int i = 0; i < houses_number; i++) {
        fin >> x; fin >> y;
        house.push_back({x, y});
    }
}

//MAIN
int main(int argc, const char * argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    srand((int)time(NULL));
    cout << setprecision(15) << fixed;
   
    //tạo toạ độ các hộ gia đình
    houses();
    
    //In ra toạ độ các hộ gia đình
    cout << "Co " << house.size() << " ho gia dinh" << endl;
    cout << endl << "Quan the da duoc tao la:" << endl << endl;
 
    //tạo quần thể ban đầu bằng random
    for(int i = 0; i < quanthe; ++i) {
        double x = Rand(1, r_area);
        double y = Rand(1, r_area);
        x /= 1000; y /= 1000;
        cout << x << " " << y << endl;
        v.push_back({x, y});
    }
    
    //In ra độ thích nghi của các cá thể
    cout << endl << "Do thich nghi cua cac ca the la:" << endl << endl;
    for(auto c : v) {
    cout << f(house, c) << endl;
    }

    //In ra xác suất tích luỹ của quần thể ban đầu
    vector<double> s = xstichluy(house, v);
    cout << endl << "Xac suat tich luy cua cac ca the ban dau lan luot la:" << endl << endl;
    for(auto c : s) cout << c << endl;
    
    //lưu cá thể max ban đầu
    pair<double, double> kq = vitri(house, v); //biến dùng để lưu cá thể max lớn nhất từng thế hệ
    pair<double, double> abcdef = vitri(house, v); //biến dùng để lưu cá thể max lớn nhất trong tất cả thế hệ
    cathemax.push_back(kq);

    //Quá trình di truyền
    while (number--) {
        
        //sắp xếp quần thể
        sort(v.begin(), v.end(), sort_v);
        
        //bánh xe xổ số
        vector<double> q = xstichluy(house, v);
        vector<pair<double, double>> v1;
        for(int i = 0; i < v.size() / 2 ; ++i) {
            double x = (double)Rand(0, 10000) / 10000;
            int vitrimin = int(lower_bound(q.begin(), q.end(), x) - q.begin());
            v1.push_back({v[vitrimin].first, v[vitrimin].second});
        }
        for(int i = 0; i < v.size() / 2 ; ++i) v[i] = v1[i];
        
        
        //tương giao chéo
        for(int i = 0; i < v.size() / 2; i += 2) {
            double x = (double)Rand(0, 10000) / 10000;
            if(x < xsditruyen) {
                double alpha = (double)Rand(0, 1000) / 1000;
                double tmp1 = v[i].first, tmp2 = v[i].second;
                double tmp3 = v[i + 1].first, tmp4 = v[i + 1].second;
                
                v[i].first = alpha * tmp1 + (1 - alpha) * tmp3;
                v[i].second = alpha * tmp2 + (1 - alpha) * tmp4;
                v[i + 1].first = alpha * tmp3 + (1 - alpha) * tmp1;
                v[i + 1].second = alpha * tmp4 + (1 - alpha) * tmp2;
            }
        }
        
        //biến dị
        for(int i = 0; i < v.size(); i++) {
            double x = (double)Rand(0, 10000) / 10000;
            if(x < xsbiendi) {
                double y = (double)Rand(0, 10000) / 10000;
                if(y < xsbiendi) {
                    v[i].first = (double)Rand(1, r_area) / 1000;
                    v[i].second = (double)Rand(1, r_area) / 1000;
                } else {
                    if(rand() % 2) {
                        v[i].first = (double)Rand(1, r_area) / 1000;
                    } else {
                        v[i].second = (double)Rand(1, r_area) / 1000;
                    }
                }
                //        c.first = (double)Rand(1, r_area) / 1000;
                //        c.second = (double)Rand(1, r_area) / 1000;
            }
        }
        
        //Lưu lại cá thể max từng thế hệ
        kq = vitri(house, v);
        cathemax.push_back(kq);
        //Lưu lại cá thể max của tất cả các thế hệ
        if(f(house, kq) > f(house, abcdef)) abcdef = kq;
    }
    
    //In ra độ thích nghi lớn nhất mỗi vòng đời
    cout << endl << "Do thich nghi lon nhat moi vong doi la" << endl;
    for(auto c : cathemax) cout << f(house, c) << endl;
    cout << endl;

    //In ra vị trí tốt nhất để đặt trạm
    cout << "Mot trong cac vi tri tot nhat de dat tram phat song la" << endl;
    cout << abcdef.first << " " << abcdef.second << " " << f(house,abcdef) << endl;

    //In tập quần thể thế hệ cuối
    cout << endl << "Quần thể thế hệ cuối:" << endl;
    for (auto c : v) cout << c.first << " " << c.second << endl;

    //Lưu tập các hộ gia đình và lời giải tối ưu giải được bằng thuật toán di truyền vào file Ditruyen.txt
    ofstream file("Ditruyen.txt");
    file << setprecision(2) << fixed << abcdef.first << " " << abcdef.second << " " << f(house, abcdef) << endl;
    file << house.size() << endl;
    for(auto c : house) {
        file << c.first << " " << c.second << endl;
    }
    
//    //Giải bằng thuật toán random
//    int maxrandom = 0;
//    pair<double, double> maxpair;
//    while (numberrandom--) {
//        double x = Rand(1, r_area);
//        double y = Rand(1, r_area);
//        x/= 1000; y/=1000;
//        pair<double, double> var = {x, y};
//        if (maxrandom < f(house, var)) {
//            maxrandom = f(house, var);
//            maxpair = var;
//        }
//    }
//    cout << endl;
//    cout << "Thuat toan random tra ve ket qua la:" << endl;
//    cout << maxpair.first << " " << maxpair.second << " " <<  maxrandom << endl;

//    //Đánh giá thuật toán random (chạy 100 lượt và lấy giá trị trung bình của lời giải nhận được sau mỗi lượt random)
//    numberrandom = 500;
//    vector<vector<int>> everyRandomResult;
//    vector<int> meanRandomResult;
//    for (int i = 0; i < 100; i++) {
//        maxrandom = 0;
//        vector<int> temp = {};
//        for (int j = 0; j < numberrandom; j++) {
//            double x = Rand(1, r_area);
//            double y = Rand(1, r_area);
//            x/= 1000; y/=1000;
//            pair<double, double> var = {x, y};
//            if (maxrandom < f(house, var)) {
//                maxrandom = f(house, var);
//            }
//            temp.push_back(maxrandom);
//        }
//        everyRandomResult.push_back(temp);
//    }
//    int randomresult = 0;
//    for (int i = 0; i < numberrandom; i++) {
//        randomresult = 0;
//        for (int j = 0; j < 100; j++) {
//            randomresult += everyRandomResult[j][i];
//        }
//        randomresult /= 100;
//        meanRandomResult.push_back(randomresult);
//    }
//
//    //In ra tập giá trị random trung bình
//    cout << endl;
//    cout << "Tập giá trị random trung bình:" << endl;
//    ofstream fout("random_result.txt");
//    for (int i = 0; i < numberrandom; i++) {
//        cout << meanRandomResult[i] << endl;
//        fout << meanRandomResult[i] << endl;
//    };
    
    //Biểu diễn tập hợp hộ gia đình và lời giải tối ưu từ thuật toán di truyền bằng SDL
    init();
    bool quit = false;
    SDL_Event e;
    srand((unsigned int)time(NULL));
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
    for (auto c : house) {
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
        SDL_RenderDrawPoint(gRenderer, c.first/r_area*100000*8, c.second/r_area*100000*8 );
    }
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
    drawCircle(gRenderer, abcdef.first/r_area*100000*8, abcdef.second/r_area*100000*8, r/r_area*100000*8);
    SDL_RenderPresent(gRenderer);

    //vòng lặp kiểm tra sự kiện
    while( !quit ) {
        while( SDL_PollEvent( &e ) != 0 ) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) quit = true;
            }
        }
    }
    close();
    
}




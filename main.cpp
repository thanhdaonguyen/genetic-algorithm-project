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

//random 1 s??? t??? l ?????n h
long long Rand(int l, int h) {
    return (long long)rand() % (h - l) + l;
}

//Khai b??o c??c tham s???, qu???n th???, d??? li???u li??n quan
const double xsditruyen = 0.7;
const double xsbiendi = 0.1;
const double r1 = 300; //b??n k??nh tr?????ng h???p n??ng th??n
const double r2 = 50; //b??n k??nh tr?????ng h???p ???? th???
const double r = r2; //b??n k??nh c???a tr???m ph??t(t??nh theo dam - ????? ca m??t)

const int r_area1 = 1500000;//????? d??i c???nh ph???m vi x??t h??nh vu??ng khu v???c n??ng th??n
const int r_area2 = 200000;//????? d??i c???nh ph???m vi x??t h??nh vu??ng khu v???c ???? th???
const int r_area = r_area2;//????n v??? cm

const int quanthe = 50;//s??? l?????ng qu???n th??? ban ?????u
int number = 20;//s??? l?????t di truy???n
int numberrandom = 500; //s??? l?????t random d??ng cho l???i gi???i random

vector<pair<double, double>> v;//t???p qu???n th???
vector<pair<double, double>> house;//t???p h??? g??
vector<pair<double, double>> cathemax;//t???p ch???a c??c c?? th??? max ??? t???ng v??ng ?????i
vector<int> average;//t???p ch???a ????? t???t trung b??nh ??? t???ng v??ng ?????i

string path = "Ditruyen2.txt";

//t??nh ????? th??ch nghi c???a t???ng c?? th??? {x, y}
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

//h??m sort cho qu???n th???
double sort_v(pair<double, double> a, pair<double, double> b) {
    return f(house, a) < f(house, b);
}

//t??m c?? th??? c?? ????? th??ch nghi l???n nh???t.
pair<double, double> max_cathe(vector<pair<double, double>> house, vector<pair<double, double>> v) {
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

//t??nh ????? t???t trung b??nh trong m???t th??? h???
double average_return(vector<pair<double, double>> house, vector<pair<double, double>> v) {
  double sum = 0;
  pair<double, double> ans = v[0];
  for(auto c : v) {
    sum += f(house, c);
  }
  sum /= v.size();
  return int(sum);
}

//t???o m???ng ch???a x??c su???t t??ch l??y c???a qu???n th???
vector<double> xstichluy(vector<pair<double, double>> house, vector<pair<double, double>> v) {
    vector<double> ans; //t???p h???p ch???a x??c su???t t??ch lu???
    vector<double> abc; //t???p h???p ch???a x??c su???t t??ch lu??? ch??a chu???n ho??
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
   
    //t???o to??? ????? c??c h??? gia ????nh
    houses();
    
    //In ra to??? ????? c??c h??? gia ????nh
    cout << "Co " << house.size() << " ho gia dinh" << endl;
    cout << endl << "Quan the da duoc tao la:" << endl << endl;
 
    //t???o qu???n th??? ban ?????u random
    for(int i = 0; i < quanthe; ++i) {
        double x = Rand(1, r_area);
        double y = Rand(1, r_area);
        x /= 1000; y /= 1000;
        cout << x << " " << y << endl;
        v.push_back({x, y});
    }
    
    //In ra ????? th??ch nghi c???a c??c c?? th???
    cout << endl << "Do thich nghi cua cac ca the la:" << endl << endl;
    for(auto c : v) {
    cout << f(house, c) << endl;
    }

    //In ra x??c su???t t??ch lu??? c???a qu???n th??? ban ?????u
    vector<double> s = xstichluy(house, v);
    cout << endl << "Xac suat tich luy cua cac ca the ban dau lan luot la:" << endl << endl;
    for(auto c : s) cout << c << endl;
    
    
    
    //l??u c?? th??? max, trung b??nh ban ?????u
    pair<double, double> kq_max = max_cathe(house, v), abc = max_cathe(house, v);
    int kq_tb = average_return(house, v);
    cathemax.push_back(kq_max);
    average.push_back(kq_tb);
    
    //Qu?? tr??nh di truy???n
    while (number--) {
        
        //s???p x???p qu???n th???
        sort(v.begin(), v.end(), sort_v);
        
        //b??nh xe x??? s???
        vector<double> q = xstichluy(house, v);
        vector<pair<double, double>> v1;
        for(int i = 0; i < v.size() / 2 ; ++i) {
            double x = (double)Rand(0, 10000) / 10000;
            int vitrimin = int(lower_bound(q.begin(), q.end(), x) - q.begin());
            v1.push_back({v[vitrimin].first, v[vitrimin].second});
        }
        for(int i = 0; i < v.size() / 2 ; ++i) v[i] = v1[i];
        
        
        //t????ng giao ch??o
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
        
        //bi???n d???
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
        
        //L??u l???i c?? th??? max v?? trung b??nh t???ng th??? h???
        kq_max = max_cathe(house, v);
        kq_tb = average_return(house, v);
        cathemax.push_back(kq_max);
        average.push_back(kq_tb);
        //L??u l???i c?? th??? max c???a t???t c??? c??c th??? h???
        if(f(house, kq_max) > f(house, abc)) abc = kq_max;
        
    }
    
    //In ra ????? th??ch nghi l???n nh???t v?? trung b??nh c???a m???i v??ng ?????i
    cout << endl << "Do thich nghi lon nhat va trung binh moi vong doi la" << endl;
    for(int i = 0; i < cathemax.size(); ++i) cout << f(house, cathemax[i]) << " " << average[i] << endl;
    cout << endl;

    //In ra v??? tr?? t???t nh???t ????? ?????t tr???m
    cout << "Mot trong cac vi tri tot nhat de dat tram phat song la" << endl;
    cout << abc.first << " " << abc.second << " " << f(house,abc) << endl;

    //L??u t???p c??c h??? gia ????nh v?? l???i gi???i t???i ??u gi???i ???????c b???ng thu???t to??n di truy???n v??o file Ditruyen.txt
    ofstream file("Ditruyen.txt");
    file << setprecision(2) << fixed << abc.first << " " << abc.second << " " << f(house, abc) << endl;
    file << house.size() << endl;
    for(auto c : house) {
        file << c.first << " " << c.second << endl;
    }
    
//    //Gi???i b???ng thu???t to??n random
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

//    //????nh gi?? thu???t to??n random (ch???y 100 l?????t v?? l???y gi?? tr??? trung b??nh c???a l???i gi???i nh???n ???????c sau m???i l?????t random)
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
//    //In ra t???p gi?? tr??? random trung b??nh
//    cout << endl;
//    cout << "T???p gi?? tr??? random trung b??nh:" << endl;
//    ofstream fout("random_result.txt");
//    for (int i = 0; i < numberrandom; i++) {
//        cout << meanRandomResult[i] << endl;
//        fout << meanRandomResult[i] << endl;
//    };
    
    //Bi???u di???n t???p h???p h??? gia ????nh v?? l???i gi???i t???i ??u t??? thu???t to??n di truy???n b???ng SDL
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
    drawCircle(gRenderer, abc.first/r_area*100000*8, abc.second/r_area*100000*8, r/r_area*100000*8);
    SDL_RenderPresent(gRenderer);

    //v??ng l???p ki???m tra s??? ki???n
    while( !quit ) {
        while( SDL_PollEvent( &e ) != 0 ) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) quit = true;
            }
        }
    }
    close();
    
}




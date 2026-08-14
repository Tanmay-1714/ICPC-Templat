#include <bits/stdc++.h>
using namespace std;

#ifdef ONPC
#include "debug_utils.h"
#else
#define dbg(...)
#endif

using namespace std;

struct node{
  int value = INT_MAX;
  int isArticulation = false;
  vector<int> adj;
};

int main() {
  int   n, m, q;
  cin >> n >> m >> q;
  vector<node> vec(n);

  for(int i = 0; i < m; i += 1){
    int a, b;
    cin >> a >> b;
    a --; b --;
    vec[a].adj.push_back(b);
    vec[b].adj.push_back(a);
  }

  vector<array<int, 2>> times(n);  
  vector<bool> visited(n);
  vector<vector<int>> child(n), rapedchild(n);

  int time = 0;

  auto dfs = [&](auto self, int curr) -> void{
    visited[curr] = true;
    times[curr][0] = time;

    for(int elem : vec[curr].adj){
      if(visited[elem]){
        vec[curr].value = min(vec[curr].value, times[elem][0]); 
        continue;
      }
      ++time;
      child[curr].push_back(elem);
      self(self, elem);
      vec[curr].value = min(vec[curr].value, vec[elem].value);
    }

    int temp = INT_MAX;
    for(int elem : child[curr]){
      if(times[elem][0] < times[curr][0]) continue;
      temp = vec[elem].value;

      if(temp >= times[curr][0] and temp != INT_MAX){
        rapedchild[curr].push_back(elem);
        vec[curr].isArticulation = true;
      }
    }


    times[curr][1] = time;
    ++time;
    return;
  };


  dfs(dfs, 0);


  if(vec[0].adj.size() > 1){
    vec[0].isArticulation = true;
  }

  dbg(rapedchild);
  for(int i = 0; i < q; i += 1){
    int a, b, c;
    cin >> a >> b >> c;
    if(a == c or b == c){
      cout << "NO" << endl;
      continue;
    }
    a --; b --; c --;

    if(!vec[c].isArticulation){
      cout << "YES" << endl;
    }

    else{
      bool flag = false;
      for(int elem : rapedchild[c]){
        if(times[elem][0] <= times[a][0] and times[elem][1] >= times[a][1]){
          if(times[elem][0] <= times[b][0] and times[elem][1] >= times[b][1]){
            cout << "YES" << endl;
          }
          else{
            cout << "NO" << endl;
          }
          flag = true;
          break;
        }
        if(times[elem][0] <= times[b][0] and times[elem][1] >= times[b][1]){
          if(times[elem][0] <= times[a][0] and times[elem][1] >= times[a][1]){
            cout << "YES" << endl;
          }
          else{
            cout << "NO" << endl;
          }
          flag = true;
          break;
        }
      }

      if(!flag) cout << "YES" << endl;
    }
  }

}

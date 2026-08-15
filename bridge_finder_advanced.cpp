#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#ifdef ONLINE_JUDGE
#define debug(...)
#else
#include <debug.hpp>
#endif
#define int long long
using namespace std;
using namespace __gnu_pbds;
const int MOD = 1e9 + 7;

template<typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag,tree_order_statistics_node_update>; 

void dfs(int node, int edgeID, int parent, vector<vector<pair<int,int>>>& adj, int& n, vector<bool>& visited, vector<int>& tin, vector<int>& low, int& timer, vector<vector<int>>& res, vector<bool>& isBridge) {
    visited[node] = true;
    tin[node] = timer++;
    int cur = LLONG_MAX;
    for(auto [child, id] : adj[node]) {
        if(id == edgeID) continue;
            if(!visited[child]) {
            // tree edge
            // recursively find low for child and then use it to find low for node 
            // low[node] stores the smallest in-time of the node with the smallest in-time in the dfs-subtree of that node
            dfs(child, id, node, adj, n, visited, tin, low, timer, res, isBridge);
            cur = min(cur, low[child]);
            } else {
                // back edge 
                // child must be an ancestor of node in the dfs-tree
                cur = min(cur, tin[child]);
            }    
    }

    low[node] = cur;

    // now that i know low[node] i can compare it with tin[parent] to see if (parent, node) is a bridge edge 
    // a back edge can never be a bridge edge 
    // a tree edge may or may not be a bridge edge 
    if(parent != -1) {
        // its not a bridge edge if and only if
        // low[node] < tin[node] 
        // equivalently low[node] <= tin[parent] 
        if(low[node] >= tin[node]) {
            res.push_back({parent, node});
            isBridge[edgeID] = true;
        }
    }
}

vector<vector<int>> bridge_finder(vector<vector<pair<int,int>>>& adj, int n, vector<bool>& isBridge) {
    // assumes nodes are labelled from 1 to n 
    // returns all the bridges as a vector of (vectors of size 2 (i.e. edges))
    // works correctly with self-loops and multi-edges, since each edge has a unique ID
    // and the "skip parent edge" check only skips the specific edge instance used to enter,
    // not all edges to that neighbor
    vector<vector<int>> res;

    vector<bool> visited(n + 1);
    vector<int> tin(n + 1);
    vector<int> low(n + 1);
    int timer = 1;

    for(int i = 1; i <= n; i++) {
        if(visited[i]) continue;
        dfs(i, 0, -1, adj, n, visited, tin, low, timer, res, isBridge);
    }
    
    return res;
}



int32_t main()
{
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int tt = 1;
    // cin >> tt;
    while (tt--) {
        int n, edges;
        cin >> n >> edges;
        vector<vector<pair<int,int>>> adj(n + 1);
        vector<bool> isBridge(edges + 1);

        int ID = 1;
        // try to assign every edge an edge ID in [1;edges]
        for(int i = 0; i < edges; i++) {
            int u,v;
            cin >> u >> v;
            adj[u].push_back({v, ID});
            adj[v].push_back({u, ID}); 
            ID++;
        }

        auto answer = bridge_finder(adj, n, isBridge);
        debug(answer); 
    }

    return 0;
}

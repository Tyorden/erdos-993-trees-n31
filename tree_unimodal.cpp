// Reads listg -e output (edge lists) on stdin; per tree computes independence polynomial
// via DP, checks unimodality. Prints violators; prints summary at EOF.
#include <cstdio>
#include <vector>
#include <cstring>
using namespace std;
typedef unsigned long long u64;
int n,m; vector<vector<int>> adj; vector<u64> in_[64], out_[64]; int par[64],order_[64];
void solve(long long idx){
  // root at 0, BFS order
  static int q[64]; int h=0,t=0; q[t++]=0; par[0]=-1; int cnt=0;
  static bool vis[64]; memset(vis,0,sizeof(bool)*n); vis[0]=1;
  while(h<t){int u=q[h++]; order_[cnt++]=u; for(int v:adj[u]) if(!vis[v]){vis[v]=1;par[v]=u;q[t++]=v;}}
  for(int i=0;i<n;i++){ in_[i].assign(2,0); in_[i][1]=1; out_[i].assign(1,1);} // in_: node used (size>=1); out_: not used
  for(int i=cnt-1;i>=1;i--){ int u=order_[i], p=par[u];
    // combine child u into p: newin = in_p * out_u ; newout = out_p * (in_u+out_u)
    vector<u64> a(in_[p].size()+out_[u].size()-1,0), b(out_[p].size()+max(in_[u].size(),out_[u].size())-1,0), s(max(in_[u].size(),out_[u].size()),0);
    for(size_t k=0;k<s.size();k++){ u64 v=0; if(k<in_[u].size())v+=in_[u][k]; if(k<out_[u].size())v+=out_[u][k]; s[k]=v;}
    for(size_t x=0;x<in_[p].size();x++)if(in_[p][x])for(size_t y=0;y<out_[u].size();y++) a[x+y]+=in_[p][x]*out_[u][y];
    for(size_t x=0;x<out_[p].size();x++)if(out_[p][x])for(size_t y=0;y<s.size();y++) b[x+y]+=out_[p][x]*s[y];
    in_[p]=a; out_[p]=b;
  }
  size_t L=max(in_[0].size(),out_[0].size()); vector<u64> poly(L,0);
  for(size_t k=0;k<L;k++){ if(k<in_[0].size())poly[k]+=in_[0][k]; if(k<out_[0].size())poly[k]+=out_[0][k]; }
  // unimodality: rises then falls
  bool falling=false;
  for(size_t k=1;k<poly.size();k++){ if(poly[k]<poly[k-1]) falling=true;
    else if(poly[k]>poly[k-1]&&falling){ printf("VIOLATION tree#%lld n=%d poly:",idx,n); for(u64 c:poly)printf(" %llu",c); printf("\n"); fflush(stdout); return; } }
}
int main(){ long long count=0; char line[4096];
  while(fgets(line,sizeof line,stdin)){
    if(line[0]=='G'||line[0]=='\n'){continue;}
    int a,b; if(sscanf(line,"%d %d",&a,&b)==2 && strchr(line,' ')&&!strchr(line+strspn(line," 0123456789"),'x')){
      // first line of a graph: "n m"
      n=a;m=b; adj.assign(n,{}); int got=0;
      while(got<m&&fgets(line,sizeof line,stdin)){ char*p=line; int x,y;
        while(sscanf(p,"%d %d",&x,&y)==2){ adj[x].push_back(y); adj[y].push_back(x); got++;
          p=strchr(p,' ')+1; p=strchr(p,' ')?strchr(p,' ')+1:p+strlen(p);
          while(*p==' ')p++; if(*p=='\n'||*p==0)break; }
      }
      count++; solve(count);
      if(count%10000000==0){fprintf(stderr,"processed %lldM\n",count/1000000);}
    }
  }
  printf("DONE trees=%lld all unimodal (no VIOLATION lines above)\n",count); return 0;}

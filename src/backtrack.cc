/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack(const Graph &data, const Graph &query,const CandidateSet &cs) : data(data), query(query), cs(cs) {
}
Backtrack::~Backtrack() {}

std::vector<Vertex> Backtrack::intersection(std::vector<Vertex> &v1, std::vector<Vertex> &v2){
    std::vector<Vertex> v3;

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::set_intersection(v1.begin(),v1.end(), v2.begin(),v2.end(), back_inserter(v3));
    return v3;
}


void Backtrack::BuildDag(){
  float Score[num_q];
  for(size_t u = 0; u < num_q ; ++u){
    Label label_u = query.GetLabel(u);
    size_t deg_u = query.GetDegree(u);
    //get number of elements in C_ini
    int C_ini = 0;
    for(size_t v = 0; v < data.GetNumVertices(); ++v){
      if(data.GetLabel(v) == label_u && data.GetDegree(v) >= deg_u){
        C_ini += 1;
      }
    }
    //get score
    // float score = C_ini / (float) deg_u;
    // Score[u] = score;
    Score[u] = C_ini;
  }

  //find root
  float min_score = Score[0];
  for(size_t s = 1; s < num_q; ++s){
    if(Score[s] < min_score){
      min_score = Score[s];
      root = s;
    }
  }

  src_vv.resize(num_q);
  parent.resize(num_q);
  dest_vv.resize(num_q); 

  //initialize visit
  bool visit[num_q];
  for(size_t vetx = 0; vetx < num_q; ++vetx){
    visit[vetx] = false;
  }

  //BFS from root
  std::queue<size_t> q;
  q.push(root);
  while(!q.empty()){
    int src = q.front();
    q.pop();
    if(!visit[src]){
      visit[src] = true;
      for(size_t nb = query.GetNeighborStartOffset(src); nb < query.GetNeighborEndOffset(src); ++nb){
        Vertex dest = query.GetNeighbor(nb);
        if(!visit[dest]){
          //what i want to do
          dest_vv[src].push_back(dest);
          src_vv[dest].push_back(src);
          //keep bfs going
          q.push(dest);
        }
      }
    }
  }
  parent = src_vv;
}

void Backtrack::Backtracking(
  std::map<size_t, size_t> M, 
  std::vector<size_t> extendable, 
  std::vector<std::vector<Vertex>> ext_cand, 
  std::vector<std::vector<Vertex>> src_vector
){
  if(M.size() == query.GetNumVertices()){
    //Report M
    count += 1;
    std::cout << "c ";
    for(auto it = M.begin(); it != M.end(); it++){
      std::cout << it->second << " ";
    }
    std::cout << "\n";

    if(count == 100000) exit(0);
    
  } else if(M.empty()){
    //for each v in candidates of root,
    // for(size_t c_idx = 0; c_idx < cs.GetCandidateSize(root); ++c_idx){
    for(size_t c_idx = cs.GetCandidateSize(root); c_idx--; ){
      Vertex v = cs.GetCandidate(root, c_idx);
      // m <- {(r,v)}
      M.clear();
      M.insert(std::make_pair(root, v));
      std::vector<std::vector<Vertex>> src_vv_prime = src_vector;
      // compute extendable, extendable_candidates
      // for(size_t idx = 0; idx < dest_vv[root].size(); ++idx){
      for(size_t idx = dest_vv[root].size(); idx--; ){
        std::vector<Vertex> src = src_vector[dest_vv[root][idx]];
        //remove src from destination's source list
        src.erase(std::remove(src.begin(), src.end(), root), src.end());
        src_vv_prime[dest_vv[root][idx]] = src;
        //if source list is empty = extendable
        if(src_vv_prime[dest_vv[root][idx]].empty()){
          auto me = dest_vv[root][idx];
          extendable.push_back(me);
          //compute vector of ext_cand[me]
          std::vector<Vertex> result;
          for(size_t p_idx = 0; p_idx < parent[me].size(); ++p_idx){
            auto p_id = parent[me][p_idx];
            std::vector<Vertex> u_p;
            //find vertices v adjacent to M[p_id] in G, 
            for(size_t nb = data.GetNeighborStartOffset(M[p_id]); nb < data.GetNeighborEndOffset(M[p_id]); ++nb){
              Vertex v = data.GetNeighbor(nb);
              //and check if v is in candidate vector of me
              bool find = false;
              for(size_t c_idx = 0; c_idx < cs.GetCandidateSize(me); ++c_idx){
                Vertex temp = cs.GetCandidate(me, c_idx);
                if(temp == v){
                  find = true;
                  break;
                }
              }
              if(find){
                u_p.push_back(v);
              }
            }
            if(p_idx == 0) result = u_p;
            else result = intersection(result, u_p);
          }
          ext_cand[me] = result;
        }
      }
      visit_d[v] = true;
      Backtracking(M, extendable, ext_cand, src_vv_prime);
      visit_d[v] = false;
    }
    
  } else {
    //get C_M(u) of all u in extendable and choose u with minimum size of C_M(u)
    if(extendable.size() < 1) {
      return;
    }
    int min_ext_c = ext_cand[extendable[0]].size();
    size_t min_ext_idx = 0;
    for(size_t e_idx = 1; e_idx < extendable.size(); ++e_idx){
      if(ext_cand[extendable[e_idx]].size() < min_ext_c){
        min_ext_c = ext_cand[extendable[e_idx]].size();
        min_ext_idx = e_idx;
      }
    }
    auto u = extendable.at(min_ext_idx);
    extendable.erase(remove(extendable.begin(), extendable.end(), u), extendable.end());
    //for each v in ext_cand[u]
    for(size_t cand_idx = 0; cand_idx < ext_cand[u].size(); ++cand_idx){
      Vertex v = ext_cand[u][cand_idx];

      if(!visit_d[v]){
        std::map<size_t,size_t> M_prime = M;
        auto extendable_prime = extendable;
        auto ext_cand_prime = ext_cand;
        std::vector<std::vector<Vertex>> src_vv_prime = src_vector;
        M_prime.insert(std::make_pair(u,v));
        // compute extendable, extendable_candidates
        
        for(size_t idx = 0; idx < dest_vv[u].size(); ++idx){
          std::vector<Vertex> src = src_vv_prime[dest_vv[u][idx]];
          //remove src from destination's source list
          src.erase(std::remove(src.begin(), src.end(), u), src.end());
          src_vv_prime[dest_vv[u][idx]] = src;
          //if source list is empty = extendable
          if(src.empty()){
            auto me = dest_vv[u][idx];
            extendable_prime.push_back(me);
            //compute ext_cand[me]
            std::vector<Vertex> result;
            for(size_t p_idx = 0; p_idx < parent[me].size(); ++p_idx){
              auto p_id = parent[me][p_idx];
              std::vector<Vertex> u_p;
              //find vertices v adjacent to M[p_id] in G, 
              for(size_t nb = data.GetNeighborStartOffset(M_prime[p_id]); nb < data.GetNeighborEndOffset(M_prime[p_id]); ++nb){
                Vertex v = data.GetNeighbor(nb);
                //and check if v is in candidate vector of me
                bool find = false;
                for(size_t c_idx = 0; c_idx < cs.GetCandidateSize(me); ++c_idx){
                  Vertex temp = cs.GetCandidate(me, c_idx);
                  if(temp == v){
                    find = true;
                    break;
                  }
                }
                if(find){
                  u_p.push_back(v);
                }
              }
              if(p_idx == 0) result = u_p;
              else {
                result = intersection(result, u_p);
              }
            }
            ext_cand_prime[me] = result;
          }
        }
        visit_d[v] = true;
        Backtracking(M_prime, extendable_prime, ext_cand_prime, src_vv_prime);
        visit_d[v] = false;
      }
    }
  }
}

void Backtrack::PrintAllMatches() {
  std::cout << "t " << query.GetNumVertices() << "\n";

  // implement your code here.
  num_d = data.GetNumVertices();
  num_q = query.GetNumVertices();

  BuildDag();

  std::map<size_t,size_t> M;
  std::vector<size_t> extendable;
  std::vector<std::vector<Vertex>> ext_cand;
  ext_cand.resize(num_q);

  visit_d = new bool[num_d];
  
  for(size_t vetx = 0; vetx < num_d; ++vetx){
    visit_d[vetx] = false;
  }

  std::vector<std::vector<Vertex>> src_vector = src_vv;
  count = 0;

  Backtracking(M, extendable, ext_cand, src_vector);

  std::cout << "final count : " << count << "\n";
}

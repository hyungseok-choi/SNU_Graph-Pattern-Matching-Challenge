/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"
#include <map>
#include <set>
#include <algorithm>

class Backtrack {
 public:
  Backtrack(const Graph &data, const Graph &query,const CandidateSet &cs);
  ~Backtrack();

  size_t root;
  size_t num_d;
  size_t num_q;
  bool* visit_d;
  int count;
  //int** DAG;

  std::vector<std::vector<Vertex>> src_vv;
  std::vector<std::vector<Vertex>> parent;
  std::vector<std::vector<Vertex>> dest_vv;

  std::vector<Vertex> intersection(std::vector<Vertex> &v1, std::vector<Vertex> &v2);

  void BuildDag();

  void Backtracking(
    std::map<size_t,size_t> M, 
    std::vector<size_t> extendable, std::vector<std::vector<Vertex>> ext_cand,  std::vector<std::vector<Vertex>> src_vector
  );

  void PrintAllMatches();

  
  const Graph& data;
  const Graph& query;
  const CandidateSet& cs;
                    
};

#endif  // BACKTRACK_H_

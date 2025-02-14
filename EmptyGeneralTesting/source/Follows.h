#pragma once

#include <vector>
#include <map>

#include "GlobalType.h"
#include "BitTable.h"

class Follows {
private:
  std::map<STMTLINE, STMTLINE> rightSiblingMap;
  std::map<STMTLINE, STMTLINE> leftSiblingMap;
  std::map<STMTLINE, STMTLINE>::iterator it;

  BitTable rightSiblingsMap;
  BitTable leftSiblingsMap;

public:
  Follows(void);
  ~Follows(void);

  void setFollows(STMTLINE first, STMTLINE second);
  void setFollowsStar(STMTLINE first, STMTLINE second);
  bool isFollows(STMTLINE first, STMTLINE second);
  bool isFollowsStar(STMTLINE first, STMTLINE second);
  STMTLINE getFollowedBy(STMTLINE currentLine);
  STMTLINE getFollowsFrom(STMTLINE currentLine);
  vector<STMTLINE> getFollowedByStar(STMTLINE currentLine);
  vector<STMTLINE> getFollowsFromStar(STMTLINE currentLine);
};

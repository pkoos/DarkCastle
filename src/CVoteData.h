#ifndef CVOTEDATA_H_
#define CVOTEDATA_H_

#include <string>
#include <vector>
#include <map>


struct SVoteData
{
  std::string answer;
  int votes;
};

class CVoteData
{
public:
  void SetQuestion(struct char_data *ch, std::string question);
  void AddAnswer(struct char_data *ch, std::string answer);
  void RemoveAnswer(struct char_data *ch, unsigned int answer);
  void StartVote(struct char_data *ch);
  void EndVote(struct char_data *ch);
  void Reset(struct char_data *ch);
  void OutToFile();
  bool HasVoted(struct char_data *ch);
  bool Vote(struct char_data *ch, unsigned int vote);
  void DisplayVote(struct char_data *ch);
  void DisplayResults(struct char_data *ch);
  bool IsActive() {return active;}
  CVoteData();
  ~CVoteData();  

private:
  bool active;
  std::string vote_question;
  std::vector<SVoteData> answers;
  int total_votes;
  std::map<std::string, bool> ip_voted;
  std::map<std::string, bool> char_voted;
};

#endif 
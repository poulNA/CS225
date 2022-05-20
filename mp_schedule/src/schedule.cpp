/**
 * @file schedule.cpp
 * Exam scheduling using graph coloring
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <unordered_map>

#include "schedule.h"
#include "utils.h"
#include <algorithm>

/**
 * Takes a filename and reads in all the text from the file
 * Newline characters are also just characters in ASCII
 * 
 * @param filename The name of the file that will fill the string
 */
std::string file_to_string(const std::string& filename){
  std::ifstream text(filename);

  std::stringstream strStream;
  if (text.is_open()) {
    strStream << text.rdbuf();
  }
  return strStream.str();
}

/**
 * Given a filename to a CSV-formatted text file, create a 2D vector of strings where each row
 * in the text file is a row in the V2D and each comma-separated value is stripped of whitespace
 * and stored as its own string. 
 * 
 * Your V2D should match the exact structure of the input file -- so the first row, first column
 * in the original file should be the first row, first column of the V2D.
 *  
 * @param filename The filename of a CSV-formatted text file. 
 */
V2D file_to_V2D(const std::string & filename){
    // Your code here!
  std::vector<std::vector<std::string>> content;
	std::vector<std::string> row;
	std::string line, word;
 
	std::fstream file (filename, std::ios::in);
	if(file.is_open())
	{
		while(getline(file, line))
		{
			row.clear();
 
			std::stringstream str(line);
 
			while(getline(str, word, ',')) {

				row.push_back(Trim(word));
      }
			content.push_back(row);
		}
	}
  return content;
}

/**
 * Given a course roster and a list of students and their courses, 
 * perform data correction and return a course roster of valid students (and only non-empty courses).
 * 
 * A 'valid student' is a student who is both in the course roster and the student's own listing contains the course
 * A course which has no students (or all students have been removed for not being valid) should be removed
 * 
 * @param cv A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param student A 2D vector of strings where each row is a student ID followed by the courses they are taking
 */
V2D clean(V2D & cv, V2D & student){
    // YOUR CODE HERE
    V2D x;
    std::string stutarget;
    std::string coursetarget;

    for (unsigned int i = 0; i < cv.size(); i++) {
      std::vector<std::string> row;
      for (unsigned int j = 0; j < cv[i].size(); j++) {
        stutarget = cv[i][j];
        coursetarget = cv[i][0];
        if (j == 0) {

          row.push_back(coursetarget);
        } else {
          for (unsigned int k = 0; k < student.size(); k++) {
            for (unsigned int l = 0; l < student[k].size(); l++) {
              if (student[k][0] == stutarget && student[k][l] == coursetarget) {
                row.push_back(stutarget);
                break;
              }
            }
          }
        }
      }
      if (row.size() > 1) {x.push_back(row);}
    }
    return x;
}

/**
 * Given a collection of courses and a list of available times, create a valid scheduling (if possible).
 * 
 * A 'valid schedule' should assign each course to a timeslot in such a way that there are no conflicts for exams
 * In other words, two courses who share a student should not share an exam time.
 * Your solution should try to minimize the total number of timeslots but should not exceed the timeslots given.
 * 
 * The output V2D should have one row for each timeslot, even if that timeslot is not used.
 * 
 * As the problem is NP-complete, your first scheduling might not result in a valid match. Your solution should 
 * continue to attempt different schedulings until 1) a valid scheduling is found or 2) you have exhausted all possible
 * starting positions. If no match is possible, return a V2D with one row with the string '-1' as the only value. 
 * 
 * @param courses A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param timeslots A vector of strings giving the total number of unique timeslots
 */
V2D schedule(V2D courses, std::vector<std::string> timeslots){
    // Your code here!
    std::vector<std::vector<int>> adjm = makeADJM(courses);
    std::map<std::string, int> m;
    std::map<int, bool> color;
    
    unsigned i;
    
    bool exit = true;
  
    for (unsigned int start = 0; start < adjm.size(); start++) {
      i = start % adjm.size();
      unsigned count = 0;
      while (count != adjm.size()) {
    
        for (unsigned int j = 0; j < adjm.size(); j++) {

          if (adjm[i][j] == 1) {

            if (m.find(courses[j][0]) != m.end()) {
              int found = m.find(courses[j][0])->second;
              color.insert({found, false});
            }
          }
        }

        // Invalid and new start
        if (color.size() >= timeslots.size()) {
      
          if (start == adjm.size()-1) {
            V2D invalid;
            std::vector<std::string> temp;
            temp.push_back("-1");
            invalid.push_back(temp);
            return invalid;
          }
            i = (i+1) % (adjm.size());
            count++;
            m.clear();
            color.clear();
            exit = false;
            break;
        }

        for (unsigned int c = 0; c < timeslots.size(); c++) {
          if (color.find(c) == color.end()) {
            m.insert({courses[i][0], c});
            break;
          }
        }

        i = (i+1) % adjm.size();
        count++;
        color.clear();
      }
      if (exit) {
        exit = true;
        break;
      }
    }

    V2D sched;
    std::vector<std::string> row;
    for (unsigned int i = 0; i < timeslots.size(); i++) {
      row.clear();
      row.push_back(timeslots[i]);
      for (std::map<std::string, int>::iterator it = m.begin(); it != m.end(); it++) {
        if ((unsigned)it->second == i) {
          row.push_back(it->first);
        }
      }
      sched.push_back(row);
    }

    return sched;
}

std::vector<std::vector<int>> makeADJM(V2D courses) {
  std::map<std::string, std::vector<int>> m;
  std::vector<std::vector<int>> adjm(courses.size(), std::vector<int>(courses.size(), 0));

  for (unsigned int i = 0; i < courses.size(); i++) {
    for (unsigned int j = 1; j < courses[i].size(); j++) {

      if (m.find(courses[i][j]) != m.end()) {
        std::vector<int> indices = m.find(courses[i][j])->second;
        for (unsigned int x = 0; x < indices.size(); x++) {
          adjm[indices[x]][i] = 1;
          adjm[i][indices[x]] = 1;
        }
      } else {
        std::vector<int> v;
        m.insert({courses[i][j], v});
      }
      m[courses[i][j]].push_back(i);
    }
  }
  return adjm;
}
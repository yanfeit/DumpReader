#ifndef DUMPREADER_H
#define DUMPREADER_H

/*

class Snapshot
class Dump

 */

/* header file
 long int strtol(const char*str, char** endptr, int base);
 bool good() const;
 bool eof const;
 istream& getline (char*s, streamsize n);
 const char* c_str() const;
*/
#include <cstdlib> /* strtol, strtod  */
#include <ios>     /* good */
#include <iostream> /* getline */
#include <fstream>
#include <sstream>
#include <string>
#include <vector> /* c_str */

//

class Snapshot{

public:
  int timestep;
  int natoms;
  std::vector<double> xbox;
  std::vector<double> ybox;
  std::vector<double> zbox;
  std::vector<int> ID;
  std::vector<int> type;
  std::vector<std::vector<double> > atomCoords;

  Snapshot();
  ~Snapshot();
  
};

Snapshot::Snapshot(){// constructor
}
Snapshot::~Snapshot(){// destructor
}


class Dump{

public:

  std::string FILE;  // FILE name
  std::ifstream infile;  
  int nsnapshots;
  std::vector<Snapshot> snapshots;
  
  Dump(const std::string);
  ~Dump();

private:
  // ....
  bool _readSnapshot();
  
};

Dump::Dump(const std::string file)
: FILE(file){

  // constructor
  infile.open(FILE.c_str());
  if (!infile.good()){
    std::cout << "NO " << FILE << " is founded!" << std::endl;
  }
  while(_readSnapshot()){}
  

  infile.close();
  nsnapshots = static_cast<int>(snapshots.size());
  
}

Dump::~Dump(){// destructor
}

bool Dump::_readSnapshot(){

  std::string line;
  char * pEnd;
  
  if (infile.eof()){
    return false;
  }

  snapshots.push_back(Snapshot());
  std::vector<Snapshot>::iterator it = snapshots.end()-1;

  // ITEM: TIMESTEP
  std::getline(infile, line);
  // 18000000
  std::getline(infile, line);
  it->timestep = std::strtol(line.c_str(), &pEnd, 10);
  // ITEM: NUMBER OF ATOMS
  std::getline(infile, line);
  // 12288
  std::getline(infile, line);
  it->natoms = std::strtol(line.c_str(), &pEnd, 10);
  // ITEM: BOX BOUNDS pp pp pp
  std::getline(infile, line);
  /* 0 93.5686
     0 93.5686
     0 93.5686 */
  std::getline(infile, line);
  it->xbox.push_back(std::strtod(line.c_str(), &pEnd));
  it->xbox.push_back(std::strtod(pEnd, 0));
  std::getline(infile, line);
  it->ybox.push_back(std::strtod(line.c_str(), &pEnd));
  it->ybox.push_back(std::strtod(pEnd, 0));
  std::getline(infile, line);
  it->zbox.push_back(std::strtod(line.c_str(), &pEnd));
  it->zbox.push_back(std::strtod(pEnd, 0));
  // ITEM: ATOMS id type xs ys zs
  std::getline(infile, line);
  it->atomCoords.resize(it->natoms);
  for (int i = 0; i < it->natoms; i++){
    
    std::getline(infile, line);
    it->ID.push_back(std::strtol(line.c_str(), &pEnd, 10));
    it->type.push_back(std::strtol(pEnd, &pEnd, 10));
    it->atomCoords[i].push_back(std::strtod(pEnd, &pEnd));
    it->atomCoords[i].push_back(std::strtod(pEnd, &pEnd));
    it->atomCoords[i].push_back(std::strtod(pEnd, 0));
    
    
  }
  
  return true;
 
}


#endif /* DUMPREADER_H */

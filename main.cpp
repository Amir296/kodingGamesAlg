
#include <iostream>
#include <list> 
#include <string>
#include <vector>
#include <algorithm>
#include <map>


using namespace std;

  
typedef pair < int, int >T_Link;

typedef struct
{
  int zId;			// this zone's ID
  int ownerId;			// the player who owns this zone (-1 otherwise)
  int podsP0;			// player 0's Phttps://www.onlinegdb.com/online_c++_compiler#tab-stdinODs on this zone
  int podsP1;			// player 1's PODs on this zone
  int myPods;
  int enemyPods;
  int visible;			// 1 if one of your units can see this tile, else 0
  int platinum;			// the amount of Platinum this zone can provide (0 if hidden by fog)
} T_Zone;

typedef struct
{
  int numOfPods;
  int zoneFrom;
  int zoneTo;
} T_Move;

typedef enum{
    R_PLATINUM = 1,
    R_INC_ENEMY_ZONE,
    R_INC_NAT_ZONE,
    R_DFS,
    R_BFS,
    
    R_ENUM_END
}E_Reson;

typedef pair< int,T_Zone > T_PAIR_ZONE;

//globals:
//vector < T_Zone > zones;
multimap< int,int > links;
map< int,T_Zone > zones_map;
vector < int > distFromMyHQ;
vector < int > distFromEnemyHQ;
vector < int > vecPlatinum ;

int myHQid, enemyHQid;
int zoneCount; // the amount of zones on the map


//vector < T_Link > links;
int myId;			// my player ID (0 or 1)
int enemyId;
bool dfsModeOn = true;

void PrintDistVector(const vector < int >& vec)
{
    for( int id = 0; id < vec.size() ; id++)
    {
        cerr << "id=" << id << " rank=" << vec[id]<< endl;
    }
}



// This class represents a directed graph using 
// adjacency list representation 
class Graph 
{ 
    int V;    // No. of vertices 
  
    // Pointer to an array containing adjacency 
    // lists 
    list<int> *adj;    
public: 
    Graph(int V);  // Constructor 
  
    // function to add an edge to graph 
    void addEdge(int v, int w);  
  
    // prints BFS traversal from a given source s 
    void BFS(int s,vector < int > & vecRanks);   
}; 
  
Graph::Graph(int V) 
{ 
    this->V = V; 
    adj = new list<int>[V]; 
} 
  
void Graph::addEdge(int v, int w) 
{ 
    adj[v].push_back(w); // Add w to v’s list. 
} 
  
void Graph::BFS(int s,vector < int > & vecRanks) 
{ 
    // Mark all the vertices as not visited 
    bool *visited = new bool[V]; 
    for(int i = 0; i < V; i++) 
        visited[i] = false; 
  
    // Create a queue for BFS 
    list<int> queue; 
  
    // Mark the current node as visited and enqueue it 
    visited[s] = true; 
    queue.push_back(s); 
  
    // 'i' will be used to get all adjacent 
    // vertices of a vertex 
    list<int>::iterator i; 
   int rank = 0;
   vecRanks[s] = rank;
    while(!queue.empty()) 
    { 
        rank++;
        // Dequeue a vertex from queue and print it 
        s = queue.front(); 
        
        queue.pop_front(); 
  
        // Get all adjacent vertices of the dequeued 
        // vertex s. If a adjacent has not been visited,  
        // then mark it visited and enqueue it 
        for (i = adj[s].begin(); i != adj[s].end(); ++i) 
        { 
            if (!visited[*i]) 
            { 
                visited[*i] = true; 
                queue.push_back(*i); 
                //cerr <<"bfs: id="<< *i << " rank= "<< rank <<endl; 
                vecRanks[*i] = rank;
            } 
        } 
    } 
} 

void PrintAllLinks()
{
    for (auto & ll:links)
    {
            cerr << "ll.first = "<< ll.first <<  " ll.second = " << ll.second << endl;
    }
}




bool wayToSort(T_Zone z1, T_Zone z2) { return z1.myPods > z2.myPods; }
bool CompareByPODS(T_PAIR_ZONE z1, T_PAIR_ZONE z2)
{
    return wayToSort(z1.second ,z2.second );
}




//2. find links of this zone
//3. move to zone with zero pods




int main()
{
    
    int playerCount; // the amount of players (always 2)
    //int myId; // my player ID (0 or 1)
    
    int linkCount; // the amount of links between all zones
    cin >> playerCount >> myId >> zoneCount >> linkCount; cin.ignore();
    cerr <<"zoneCount= " << zoneCount <<" linkCount = " << linkCount << endl; 
    enemyId = 1 - myId;
    Graph g(zoneCount); 
    distFromMyHQ.reserve(zoneCount);
    distFromEnemyHQ.reserve(zoneCount);
    vecPlatinum.reserve(zoneCount); 
    for(auto p : vecPlatinum)
        p = 0;
    for (int i = 0; i < zoneCount; i++) {
        int zoneId; // this zone's ID (between 0 and zoneCount-1)
        int platinumSource; // Because of the fog, will always be 0
        cin >> zoneId >> platinumSource; cin.ignore();
    }
    for (int i = 0; i < linkCount; i++) {
        int zone1;
        int zone2;
        cin >> zone1 >> zone2; cin.ignore();
        links.insert(pair<int,int>(zone1,zone2));
        links.insert(pair<int,int>(zone2,zone1));
        
        g.addEdge(zone1, zone2); 
        g.addEdge(zone2, zone1); 
        //cerr <<  zone1<<"ad " << zone2 << endl;
    }





    // game loop
    int turnNumber = 1;
    while (1) 
    {
        int myPlatinum; // your available Platinum
        cin >> myPlatinum; cin.ignore();
        cerr << "myPlatinum = " << myPlatinum<<endl;
        for (int i = 0; i < zoneCount; i++)
        {
           T_Zone tz;
            int zId; // this zone's ID
            int ownerId; // the player who owns this zone (-1 otherwise)
            int podsP0; // player 0's PODs on this zone
            int podsP1; // player 1's PODs on this zone
            int visible; // 1 if one of your units can see this tile, else 0
            int platinum; // the amount of Platinum this zone can provide (0 if hidden by fog)
            //cin >> zId >> ownerId >> podsP0 >> podsP1 >> visible >> platinum; cin.ignore();
            cin >> tz.zId >> tz.ownerId >> tz.podsP0 >> tz.podsP1 >> tz.visible >> tz.platinum; cin.ignore();
            
            tz.myPods = (0 == myId) ? tz.podsP0 : tz.podsP1;
            tz.enemyPods = (1 == myId) ? tz.podsP0 : tz.podsP1;
	        zones_map[tz.zId] = tz; //.insert(pair<int,int>(zone1,zone2));
	       // zones.push_back(tz) ;
	       
	       if(tz.platinum > 0 && 0 == vecPlatinum[tz.zId]) 
	       //update platinuim diterbution with a new zone
	       {
	        vecPlatinum[tz.zId] = tz.platinum;
	        cerr << "****  update new zone with platinuim **" << endl;
	       }
	        if(1 == turnNumber)//first turn
	        {
	            if(tz.myPods > 0)
	                myHQid = tz.zId;
	           if(tz.enemyPods > 0)
	                enemyHQid = tz.zId;
	       
	        }
	        //cerr <<"zId=" << tz.zId << " platinum=" << tz.platinum << " owId="<<tz.ownerId<<
	        //" myPods="<<tz.myPods<<" enemyPods="<<tz.enemyPods<<endl;
	        
        }
        cerr << "myHQid = " << myHQid << "enemyHQid = " << enemyHQid << endl;
        if(1 == turnNumber)//first turn
	        {
	           
	                
	           //run bfs on rank each zone by dist to my HQ  
	           
	           g.BFS(myHQid,distFromMyHQ); 
	           g.BFS(enemyHQid,distFromEnemyHQ);
	           cerr << "--- distFromEnemyHQ -- " <<endl;
	           PrintDistVector(distFromEnemyHQ);
	        }

        //sort(zones.begin(),zones.end(),wayToSort);
        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;


        // first line for movement commands, second line no longer used (see the protocol in the statement for details)
        int num_of_moves = 0;
        //for (auto & zz:zones)           
        
        dfsModeOn = zones_map[enemyHQid].enemyPods < 50;
        map< int,bool > mapIsPodMoveTo; 
       for(int  zone_id = 0; zone_id <  zoneCount; zone_id++ )
        {
                mapIsPodMoveTo[zone_id] = false;
        }
        for(int zId = 0 ; zId < zoneCount ; zId++)
        {
            cerr <<"zFromId=" << zId<<endl;
            int movesFromThisZone = 0;
            auto zzFrom = zones_map[zId];
            if (zzFrom.ownerId != myId || zzFrom.myPods == 0 || zzFrom.platinum > 1 && zzFrom.myPods == 1)
            {
                continue;
            }
            
            auto relevant_links = links.equal_range(zzFrom.zId);
            vector<T_Zone> neighbors;

            for (auto it = relevant_links.first; it != relevant_links.second; it++)
            {
                int targetZoneId = it->second;
            
                T_Zone zzDst = zones_map[targetZoneId];
                neighbors.push_back(zzDst);
                sort(neighbors.begin(),neighbors.end(),wayToSort); // most pods at start
            }

            E_Reson er = R_PLATINUM;
            

            

            for(int i_er = R_PLATINUM; i_er < R_ENUM_END; i_er++)
            {
                if( 0 == zzFrom.myPods)
                {
                    break;
                }
                
                cerr <<"i_er=" << i_er<<" num neighbors= " << neighbors.size()<<"zzFrom.myPods" <<zzFrom.myPods<< endl;
                T_Zone zzDest;
                for(int n = neighbors.size()-1 ; n > -1  ; n--)
                {
                  if( 0 == zzFrom.myPods)
                    {
                    break;
                    }   
                bool cond = false;  
                cerr <<"a zzDest=" << zzDest.zId<<endl;
                er = (E_Reson)i_er;
                zzDest = neighbors[n];
                if(true == mapIsPodMoveTo[zzDest.zId])
                    {
                        continue;
                    }
                
                cerr <<"b zzDest=" << zzDest.zId<<endl;
                
                switch(er) 
                    {
                    case R_PLATINUM:
                        cond = zzDest.platinum > 0 && zzDest.ownerId != myId;
                        break;
                    case R_DFS:
                        cond = dfsModeOn && distFromEnemyHQ[zzDest.zId] < distFromEnemyHQ[zzFrom.zId];
                        break;
                    case R_BFS:
                        cond = distFromMyHQ[zzDest.zId] >= distFromMyHQ[zzDest.zId];
                        break;
                    case R_INC_NAT_ZONE:
                        cond = zzDest.ownerId == -1;
                        break;
                    case R_INC_ENEMY_ZONE:
                        cond = zzDest.ownerId == enemyId;
                        break;
                    default:
                        cond = false;
                    }
            
                    if(cond)
                    {
                        movesFromThisZone++;
                        T_Move tm;
                        if(R_DFS == er && zzFrom.myPods >4 || zzFrom.myPods > 20)
                        {
                            tm.numOfPods = zzFrom.myPods/4 +1 ;    
                        }
                        else
                            tm.numOfPods = 1;
                        zzFrom.myPods -= tm.numOfPods;
                        tm.zoneTo = zzDest.zId;
                        mapIsPodMoveTo[zzDest.zId] = true;
                        tm.zoneFrom = zzFrom.zId;
                        num_of_moves++;
                        //cerr<<"num_of_moves="<<num_of_moves << endl;
                        cerr <<"reson = "<< er<<" cmnd:"<< tm.numOfPods << " " << tm.zoneFrom <<" " << tm.zoneTo <<" " <<endl;
                        cout << tm.numOfPods << " " << tm.zoneFrom <<" " << tm.zoneTo <<" " ;
                  }
                }//for all neughbors
            } //all resons
            cerr << "movesFromThisZone= " << movesFromThisZone<< endl;
            
            
        
        }
        cerr << "num_of_moves = " << num_of_moves << endl;
        if(0 == num_of_moves)
        {
        cout << "WAIT" << endl;    
        }
        else
        {
            cout << endl;
        }
        
        cout << "WAIT" << endl;
        
        turnNumber++;
        cerr << "-------turnNumber=" << turnNumber << "------"<<endl;
    }
}

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;

struct compare
{
    bool operator()(pair<int, int> a, pair<int, int> b)
    {
        if (a.first == b.first)
        {
            return a.second > b.second;
        }

        return a.first > b.first;
    }
};

int graph[101][101];
vector<pair<int, int> > problems;
vector<string> message_contents;

// 벨만 포드 dist vector
void solve(ofstream &of, int n)
{
    int dist[101][101];
    int path[101][101];
    for (int start = 0; start < n; ++start)
    {

        for (int i = 0; i < 101; ++i)
        {
            path[start][i] = 0;
            dist[start][i] = 1e9 + 7;
        }

        dist[start][start] = 0;
        for(int k=0;k<n;++k){
            for(int i=0;i<n;++i){
                for(int j=0;j<n;++j){
                    if(graph[i][j] + dist[start][i] < dist[start][j] && graph[i][j] > 0){
                        dist[start][j] = dist[start][i] + graph[i][j];
                        path[start][j] = i;
                    }
                }
            }
        }
        
        for (int i = 0; i < n; ++i)
        {
            if (dist[start][i] < 0 || dist[start][i] == 1e9 + 7)
            {
                continue;
            }
            if (i == start)
            {
                of << i << " " << i << " " << dist[start][i] << "\n";
                continue;
            }
            vector<int> res;
            int end = i;
            while (1)
            {
                res.push_back(end);
                end = path[start][end];
                if (end == start)
                {
                    break;
                }
            }
            of << i << " " << res.back() << " " << dist[start][i] << "\n";
        }
        of << "\n";
    }

    for (int i = 0; i < (int)problems.size(); ++i)
    {

        int s = problems[i].first;
        int e = problems[i].second;
        if (dist[s][e] <= 0 || dist[s][e] == 1e9 + 7)
        {
            of << "from " << s << " to " << e << " cost infinite hops unreachable message " << message_contents[i] << "\n";
            continue;
        }
        else
        {
            of << "from " << s << " to " << e << " cost " << dist[s][e] << " hops ";
            vector<int> res;
            int end = e;
            while (1)
            {
                res.push_back(end);
                end = path[s][end];
                if (end == s)
                {
                    res.push_back(end);
                    break;
                }
            }
            reverse(res.begin(), res.end());
            res.pop_back();

            for(int i = 0; i< (int)res.size(); ++i){
                of << res[i] << " ";
            }
            of << "message " << message_contents[i] << "\n";
        }
    }
    of << "\n";
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "usage: distvec topologyfile messagesfile changesfile\n";
        return 1;
    }

    string topology_file_name = argv[1];
    string message_file_name = argv[2];
    string changes_file_name = argv[3];

    ifstream topology_file(topology_file_name.data());

    if (!topology_file.is_open())
    {
        printf("Error: open input file.\n");
        topology_file.close();
        return 1;
    }

    ifstream message_file(message_file_name.data());

    if (!message_file.is_open())
    {
        printf("Error: open input file.\n");
        message_file.close();
        return 1;
    }

    ifstream changes_file(changes_file_name.data());

    if (!changes_file.is_open())
    {
        printf("Error: open input file.\n");
        changes_file.close();
        return 1;
    }

    int n;
    topology_file >> n;
    string tmp;
    while(getline(topology_file, tmp)){
        stringstream ss(tmp);
        int a, b, w;
        ss >> a >> b >> w;
        graph[a][b] = w;
        graph[b][a] = w;
    }

    while (getline(message_file, tmp))
    {

        int a, b;
        string message = "";
        stringstream ss(tmp);
        string buffer;
        int cnt = 0;
        while (getline(ss, buffer, ' '))
        {
            if (cnt == 0)
            {
                a = stoi(buffer);
            }
            else if (cnt == 1)
            {
                b = stoi(buffer);
            }
            else
            {
                if (cnt == 2)
                {
                    message = buffer;
                }
                else
                {
                    message += " " + buffer;
                }
            }
            cnt++;
        }
        problems.push_back(make_pair(a, b));
        message_contents.push_back(message);
    }

    // solve
    ofstream result_file("output_dv.txt");
    solve(result_file, n);
    while (getline(changes_file, tmp))
    {
        int a, b, w;
        stringstream stream;
        stream.str(tmp);
        stream >> a >> b >> w;
        graph[a][b] = w;
        graph[b][a] = w;

        solve(result_file, n);
    }

    // done
    cout << "Complete. Output file written to output_dv.txt\n";
}

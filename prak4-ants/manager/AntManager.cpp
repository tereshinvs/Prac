#include "AntManager.hpp"
#include <memory.h>

//
//  AntManager
//

void init_vector_vector(std::vector< std::vector<int> > &t, int w, int h) {
    t.resize(w);
    for (int i = 0; i < w; i++)
        t[i].resize(h);    
}

AntManager::AntManager(int height, int width, int teamCount, int maxAntCountPerTeam)
{
	this->height = height;
	this->width = width;
	this->teamCount = teamCount;
	this->maxAntCountPerTeam = maxAntCountPerTeam;

    cur_teamid = cur_ant = 0;

    init_vector_vector(field, width, height);
    init_vector_vector(smell, width, height);
    init_vector_vector(smell_intensity, width, height);

    score.resize(teamCount);

    ant_teams.resize(teamCount);
    angles.push_back(antgui::Point(0, 0));
    angles.push_back(antgui::Point(width - 1, 0));
    angles.push_back(antgui::Point(0, height - 1));
    angles.push_back(antgui::Point(width - 1, height - 1));
    for (int j = 0; j < teamCount; j++) {
        for (int i = 0; i < maxAntCountPerTeam; i++)
            ant_teams[j].push_back(antlogic::AntImpl(false, false, angles[j], j));
        logic.push_back(antlogic::IAntLogic::GetAntLogic(j));
    }
}

bool AntManager::is_there_team_ant_at_point(int teamid, const antgui::Point &p) const {
    for (unsigned i = 0; i < ant_teams[teamid].size(); i++)
        if (p.x == ant_teams[teamid][i].getPoint().x &&
            p.y == ant_teams[teamid][i].getPoint().y)
            return true;
    return false;
}

bool AntManager::is_correct_point(const antgui::Point &p) const {
    return p.x >= 0 && p.y >= 0 && p.x < width && p.y < height;
}

void AntManager::get_sensors(const antgui::Point &ant_point, int teamid, antlogic::AntSensor sensors[3][3]) const {
    for (int q = 0; q < 3; q++)
        for (int w = 0; w < 3; w++) {
            antgui::Point curp(ant_point.x + q - 1, ant_point.y + w - 1);
            if (!is_correct_point(curp)) {
                sensors[q][w].isWall = true;
                continue;
            }
            sensors[q][w].isWall = false;
            sensors[q][w].smell = smell[curp.x][curp.y];
            sensors[q][w].smellIntensity = smell_intensity[curp.x][curp.y];
            sensors[q][w].isFriend = is_there_team_ant_at_point(teamid, curp);
            sensors[q][w].isEnemy = false;
            for (int r = 0; r < teamCount; r++) {
                if (r == teamid)
                    continue;
                sensors[q][w].isEnemy |= is_there_team_ant_at_point(r, curp);
            }
            sensors[q][w].isFood = field[curp.x][curp.y];

            if (curp.x == 0 && curp.y == 0) {
                sensors[q][w].isMyHill = teamid == 0;
                sensors[q][w].isEnemyHill = teamid != 0;
            }
            if (curp.x == width - 1 && curp.y == 0) {
                sensors[q][w].isMyHill = teamid == 1;
                sensors[q][w].isEnemyHill = teamid != 1;
            }
            if (curp.x == 0 && curp.y == height - 1) {
                sensors[q][w].isMyHill = teamid == 2;
                sensors[q][w].isEnemyHill = teamid != 2;
            }
            if (curp.x == width - 1 && curp.y == height - 1) {
                sensors[q][w].isMyHill = teamid == 3;
                sensors[q][w].isEnemyHill = teamid != 3;
            }
        }
}

void AntManager::try_move(antlogic::AntImpl &ant, int dx, int dy) {
    antgui::Point tmp(ant.getPoint().x + dx, ant.getPoint().y + dy);
    if (is_correct_point(tmp))
        ant.setPoint(tmp);
}

antlogic::AntImpl &AntManager::get_victim(antgui::Point &p, int teamid) {
    int min_f = 100000, min_f_id = -1, min_f_num = -1;
    for (int i = 0; i < teamCount; i++) {
        if (i == teamid)
            continue;
        for (unsigned j = 0; j < ant_teams[i].size(); j++) {
            if (ant_teams[i][j].getPoint().x != p.x ||
                ant_teams[i][j].getPoint().y != p.y)
                continue;
            if (!ant_teams[i][j].isFrozen())
                return ant_teams[i][j];
            if (ant_teams[i][j].getFrozenTime() < min_f) {
                min_f = ant_teams[i][j].getFrozenTime();
                min_f_id = i;
                min_f_num = j;
            }
        }
    }
    if (min_f == 100000)
        throw "No such ant";
    else
        return ant_teams[min_f_id][min_f_num];
}

void AntManager::try_bite(antlogic::AntImpl &ant, int dx, int dy) {
    antgui::Point tmp(ant.getPoint().x + dx, ant.getPoint().y + dy);
    if (is_correct_point(tmp))
        try {
            antlogic::AntImpl &vict = get_victim(tmp, ant.getTeamId());
            vict.setFrozen(8);
            if (vict.hasFood()) {
                field[vict.getPoint().x][vict.getPoint().y]++;
                vict.setFood(false);
            }
            #ifdef DEBUG
            std::cout << "Bite: " << vict.getTeamId() << " by " << ant.getTeamId() << std::endl;
            std::cout << "Victim: " << vict.getPoint().x << " " << vict.getPoint().y << std::endl;
            #endif
        } catch (...) {
        }
}

void AntManager::update_ant(antlogic::AntImpl &ant, antlogic::AntAction action) {
    switch (action.actionType) {
        case antlogic::MOVE_UP:
            try_move(ant, 0, -1);
            break;
        case antlogic::MOVE_LEFT:
            try_move(ant, -1, 0);
            break;
        case antlogic::MOVE_DOWN:
            try_move(ant, 0, 1);
            break;
        case antlogic::MOVE_RIGHT:
            try_move(ant, 1, 0);
            break;
        case antlogic::BITE_UP:
            try_bite(ant, 0, -1);
            break;
        case antlogic::BITE_LEFT:
            try_bite(ant, -1, 0);
            break;
        case antlogic::BITE_DOWN:
            try_bite(ant, 0, 1);
            break;
        case antlogic::BITE_RIGHT:
            try_bite(ant, 1, 0);
            break;
        case antlogic::GET:
            #ifdef DEBUG
            std::cout << "Get: " << ant.getTeamId() << std::endl;
            #endif
            if (ant.hasFood() || field[ant.getPoint().x][ant.getPoint().y] == 0)
                break;
            ant.setFood(true);
            field[ant.getPoint().x][ant.getPoint().y]--;
            break;
        case antlogic::PUT:
            if (!ant.hasFood())
                break;
            ant.setFood(false);
            if (angles[ant.getTeamId()].x == ant.getPoint().x &&
                angles[ant.getTeamId()].y == ant.getPoint().y) {
                score[ant.getTeamId()]++;
                #ifdef DEBUG
                std::cout << "Score: " << ant.getTeamId() << std::endl;
                #endif
            } else
                field[ant.getPoint().x][ant.getPoint().y]++;
            break;
        default:
            break;
    }
}

void AntManager::step(int step)
{
    //
    //  Drawing
    //
    gui->BeginPaint();

    for (int i = 0; i < teamCount; i++)
        for (unsigned j = 0; j < ant_teams[i].size(); j++)
            gui->SetAnt(antgui::ConcreteAnt(ant_teams[i][j].hasFood(), ant_teams[i][j].isFrozen(),
                ant_teams[i][j].getPoint(), ant_teams[i][j].getTeamId()));

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            gui->SetFood(antgui::ConcreteFood(antgui::Point(i, j), field[i][j]));

    gui->EndPaint();

    //
    //  Updating
    //

    #ifdef DEBUG
    std::cout << cur_teamid << " " << cur_ant << "    " <<
        ant_teams[cur_teamid][cur_ant].getPoint().x << " " <<
        ant_teams[cur_teamid][cur_ant].getPoint().y << std::endl;
    #endif

    if (!ant_teams[cur_teamid][cur_ant].isFrozen()) {
        antgui::Point ant_point = ant_teams[cur_teamid][cur_ant].getPoint();
//        std::shared_ptr<antlogic::IAntLogic> logic = antlogic::IAntLogic::GetAntLogic(cur_teamid);

        antlogic::AntSensor sensors[3][3];
        get_sensors(ant_point, cur_teamid, sensors);

        antlogic::AntAction action = logic[cur_teamid].get()->GetAction(
            ant_teams[cur_teamid][cur_ant], sensors);

        if (action.putSmell) {
            smell[ant_point.x][ant_point.y] = action.smell;
            smell_intensity[ant_point.x][ant_point.y] = 100;
        }

        update_ant(ant_teams[cur_teamid][cur_ant], action);
    }

    cur_ant = (cur_ant + 1) % maxAntCountPerTeam;
    if (cur_ant == 0) {
        cur_teamid = (cur_teamid + 1) % teamCount;
        if (cur_teamid == 0) {
            for (int i = 0; i < teamCount; i++)
                for (int j = 0; j < maxAntCountPerTeam; j++)
                    ant_teams[i][j].decreaseFrozenTime();
            for (int i = 0; i < width; i++)
                for (int j = 0; j < height; j++)
                    if (smell_intensity[i][j] > 0)
                        smell_intensity[i][j]--;
        }
    }
        
    // scoreboard

    for (int t = 0; t < teamCount; t++)
    {
        gui->SetTeamScore(t, score[t]);

    }
}

void AntManager::setGui(std::shared_ptr<antgui::IAntGui> gui)
{
	this->gui = gui;
}

void AntManager::setFoodGeneretor(std::shared_ptr<food_iterator> it)
{
//    gui->BeginPaint();
    food_iterator end(0, true);
    while (*it != end) {
        antgui::ConcreteFood food = **it;
        field[food.getPoint().x][food.getPoint().y] = food.getCount();
    }
//    gui->EndPaint();
}

//
//  food_iterator
//

food_iterator::food_iterator(int size, bool _end): size(size), end(_end) {
    if (end)
        return;

    field.resize(size);
    for (int i = 0; i < size; i++)
        field[i].resize(size);

    q.push(antgui::Point(size / 2, size / 2));
    field[size / 2][size / 2] = 100;
}

bool food_iterator::operator==(const food_iterator &t) const {
    return end == t.end;
}

bool food_iterator::operator!=(const food_iterator &t) const {
    return end != t.end;
}

void food_iterator::try_put(const antgui::Point &next, const antgui::Point &cur) {
    if (is_correct_point(next) && count_neighours(next) == 1) {
        field[cur.x][cur.y] += (field[next.x][next.y] = 40) + 20;
        field[next.x][next.y] += (field[next.x - 1][next.y] = 10);
        field[next.x][next.y] += (field[next.x + 1][next.y] = 10);
        field[next.x][next.y] += (field[next.x][next.y - 1] = 10);
        field[next.x][next.y] += (field[next.x][next.y + 1] = 10);
        q.push(next);
    }    
}

bool food_iterator::is_correct_point(const antgui::Point &t) const {
    return t.x >= 1 && t.y >= 1 && t.x < size - 1 && t.y < size - 1;
}

int food_iterator::count_neighours(const antgui::Point &t) const {
    int res = 0;
    if (is_correct_point(antgui::Point(t.x, t.y)) && field[t.x][t.y] != 0)
        res++;
    if (is_correct_point(antgui::Point(t.x - 3, t.y)) && field[t.x - 3][t.y] != 0)
        res++;
    if (is_correct_point(antgui::Point(t.x + 3, t.y)) && field[t.x + 3][t.y] != 0)
        res++;
    if (is_correct_point(antgui::Point(t.x, t.y - 3)) && field[t.x][t.y - 3] != 0)
        res++;
    if (is_correct_point(antgui::Point(t.x, t.y + 3)) && field[t.x][t.y + 3] != 0)
        res++;
    return res;
}

food_iterator &food_iterator::operator++() {
    if (end)
        return *this;
    antgui::Point cur;
    if (q.empty()) {
        end = true;
        return *this;
    }
    cur = q.front(); q.pop();
    try_put(antgui::Point(cur.x - 3, cur.y), cur);
    try_put(antgui::Point(cur.x + 3, cur.y), cur);
    try_put(antgui::Point(cur.x, cur.y - 3), cur);
    try_put(antgui::Point(cur.x, cur.y + 3), cur);
    if (q.empty())
        end = true;
    return *this;
}

food_iterator food_iterator::operator++(int) {
    food_iterator res(*this);
    ++(*this);
    return res;
}

antgui::ConcreteFood food_iterator::operator*() {
    antgui::Point cur;
    cur = q.front();
    antgui::ConcreteFood res; //(cur, field[cur.x][cur.y]);
    switch (last_dir) {
        case 0: res = antgui::ConcreteFood(cur, field[cur.x][cur.y]); break;
        case 1: res = antgui::ConcreteFood(antgui::Point(cur.x - 1, cur.y), field[cur.x - 1][cur.y]); break;
        case 2: res = antgui::ConcreteFood(antgui::Point(cur.x + 1, cur.y), field[cur.x + 1][cur.y]); break;
        case 3: res = antgui::ConcreteFood(antgui::Point(cur.x, cur.y - 1), field[cur.x][cur.y - 1]); break;
        case 4: res = antgui::ConcreteFood(antgui::Point(cur.x, cur.y + 1), field[cur.x][cur.y + 1]); break;
    }
    last_dir = (last_dir + 1) % 5;
    if (last_dir == 0)
        ++(*this);
    return res;
}

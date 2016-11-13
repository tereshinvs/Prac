#ifndef ANTMANAGER_HPP
#define ANTMANAGER_HPP

#include "IAntGUI.hpp"
#include "IAntLogic.hpp"
#include "IAntLogicImpl.hpp"
#include "IAntGuiImpl.hpp"

#include <memory>
#include <iterator>
#include <queue>
#include <vector>

class food_iterator: public std::iterator< std::input_iterator_tag, antgui::ConcreteFood> {
    int size;
    bool end;
    std::queue<antgui::Point> q;
    std::vector< std::vector<int> > field;
    int last_dir = 0;
public:
    food_iterator(int size, bool _end = false);
    bool operator==(const food_iterator &t) const;
    bool operator!=(const food_iterator &t) const;
    food_iterator &operator++();
    food_iterator operator++(int);
    antgui::ConcreteFood operator*();
    void try_put(const antgui::Point &next, const antgui::Point &cur);
    bool is_correct_point(const antgui::Point &t) const;
    int count_neighours(const antgui::Point &t) const;
};

class AntManager
{
    int maxAntCountPerTeam;
    int teamCount;
    int height;
    int width;
    std::shared_ptr<antgui::IAntGui> gui;
    std::vector< std::vector<antlogic::AntImpl> > ant_teams;
    std::vector< std::vector<int> > field, smell, smell_intensity;
    std::vector<antgui::Point> angles;
    std::vector<int> score;
    //std::vector< std::vector<std::shared_ptr<antlogic::IAntLogic> > > logic;
    std::vector< std::shared_ptr<antlogic::IAntLogic> > logic;

    int cur_teamid = 0, cur_ant = 0;

    bool is_there_team_ant_at_point(int teamid, const antgui::Point &p) const;
    void get_sensors(const antgui::Point &p, int teamid, antlogic::AntSensor sensors[3][3]) const;
    bool is_correct_point(const antgui::Point &t) const;
    void update_ant(antlogic::AntImpl &ant, antlogic::AntAction action);
    void try_move(antlogic::AntImpl &ant, int dx, int dy);
    void try_bite(antlogic::AntImpl &ant, int dx, int dy);
    antlogic::AntImpl &get_victim(antgui::Point &p, int teamid);

public:
    AntManager(int height,
               int width,
               int teamCount,
               int maxAntCountPerTeam);

    void step(int iRun);
    void setGui(std::shared_ptr<antgui::IAntGui> gui);
    void setFoodGeneretor(std::shared_ptr<food_iterator> it);
};

#endif

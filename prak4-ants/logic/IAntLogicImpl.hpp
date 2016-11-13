#include "IAntLogic.hpp"
#include "IAntGUI.hpp"

#ifndef IANTLOGICIMPL_HPP
#define IANTLOGICIMPL_HPP

	#define id ant.getMemory()[0]
	#define x ant.getMemory()[1]
	#define y ant.getMemory()[2]
	#define job ant.getMemory()[3]
	#define lastkick ant.getMemory()[4]
	#define dist ant.getMemory()[5]
	#define dat sensors, ant, res
	#define datdef AntSensor sensors[3][3], const antlogic::Ant &ant, AntAction &res
	#define MYSMELL 362342


namespace antlogic
{
	class AntImpl: public Ant {
        mutable char mem[32];
        bool food;
        bool frozen;
        int frozen_time;
        mutable antgui::Point p;
        int teamid;

        public:
            AntImpl(bool food, bool frozen, antgui::Point &_p, int teamid):
            food(food), frozen(frozen), frozen_time(0), p(_p), teamid(teamid) {}
            virtual char *getMemory() const;
            virtual bool hasFood() const;
            virtual bool isFrozen() const;
            virtual int getFrozenTime() const;
            virtual int getTeamId() const;
            virtual antgui::Point &getPoint() const;

            virtual void setPoint(const antgui::Point &p) const;
            virtual void setFrozen(int f_time);
            virtual void setFood(bool _food);
		
		    void decreaseFrozenTime();
            virtual ~AntImpl() {}
    };

	enum Teams{Team01 = 0, Team02};

	class AntLogicTeam01 : public IAntLogic
	{
	public:
		AntAction GetAction(const Ant&, AntSensor[][3]);
	};

	class AntLogicTeam02 : public IAntLogic
	{
		int l, r, u, d;
		int cr;
		void go( datdef, int dir );
		void kick( datdef, int dir );
		AntSensor gss( datdef, int dir );
		AntSensor mgss(datdef, int gdir, int vdir);
		long getEnemyLoc( datdef );
		bool nearHill( datdef );
		int addx(int xx, int dir);
		int addy(int xx, int dir);
		int getpri(datdef, int dir);
		void gogo( datdef );
		void farmers( datdef );
		AntAction GetAntAction(AntSensor sensors[3][3], bool hasFood, char memory[MAX_MEMORY]);

	public:
		AntLogicTeam02() : cr(100) {}
		AntAction GetAction(const Ant&, AntSensor[][3]);
	};


	class AntLogicTeam03 : public IAntLogic
	{
	public:
		AntAction GetAction(const Ant&, AntSensor[][3]);
	};

}

	#undef id
	#undef x
	#undef y
	#undef job
	#undef lastkick
	#undef dist
	#undef dat
	#undef datdef
	#undef MYSMELL
#endif

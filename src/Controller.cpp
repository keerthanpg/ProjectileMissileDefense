#include "Controller.h"
#include <cmath>
#include <iostream>
namespace mrsd
{
	void Controller::control(const mrsd::Game& g, float t)
	{	
			float currentTime;
			int val;
			
			if(!p==0 and !p->dead)
			{
				determineSafeSpots(g);
				val=pickSafeSpot(g);
				if(val==1)
				{
					std::cout<<"\ngoright";
					p->x=p->x+g.playerSpeed;
				}
				else if (val==-1)
				{
					std::cout<<"\ngoleft";
					p->x=p->x-g.playerSpeed;
				}
				else
				{
					std::cout<<"\nstaystill";
				}

				//std::cout<<" X="<<p->x<<"\n";
				std::cout<<"\nTime= "<<g.getGameTime();
				/*
				for(int i=0;i<200;i++){
					std::cout<<W[0];

				}*/
				
			
			}


	}

	void Controller::createPlayer(Game& g)
	{
		if(p == 0)
		{
			p = new Player();
			p->dead = true;
		}
		if(p->dead)
		{
			p->dead = false;
			p->x = g.getWidth()/2;
			g.newPlayer(p);
		}
	}

	Prediction Controller::trackProjectile(const Projectile& p, const Game& g)
	{
		Prediction pred;
		if(p.vy>0)
		{
			float timetilltop=-p.vy/g.getGravity();//not good to declare each time
			float distilltop=-p.vy*p.vy/(2*g.getGravity());//call fuction or assign it each time?
			pred.t=sqrt(-(p.y+distilltop)/g.getGravity())+timetilltop;
			pred.x=p.x+p.vx*pred.t;
		}
		else
		{
			float b=p.vy;
			float a=0.5*g.getGravity();
			float c=p.y;
			float t1=(-b+sqrt(b*b-4*a*c))/(2*a);
			float t2=(-b-sqrt(b*b-4*a*c))/(2*a);
			if(t1>t2)
			{
				pred.t=t1;
			}
			else
			{
				pred.t=t2;
			}
			pred.x=p.x+p.vx*pred.t;

		}
		return pred;
	}

	void Controller::determineSafeSpots(const Game& g)
	{
		
		int i,j;
		int explosionSize=std::ceil(g.explosionSize);
		Prediction pred;
		for(i=0;i<200;i++)
		{
			W[i]=1;
		}
		
		for (std::list<Projectile>::const_iterator iter = g.getProjectiles().begin(); iter != g.getProjectiles().end(); ++iter)//will it read the last one?
  		{
  			//std::cout<<" Entered Projectile Loop, predicted time= "<<pred.t<<"\n";
  			pred=Controller::trackProjectile(*iter, g);
  			if(pred.t<10)
  			{
  				i=std::round(pred.x);
  				for(j=i-explosionSize;j<=i+explosionSize;j++)//check if explosionSize is indeed radius
  				{
  					if(j>=0 && j<200)
  					{
  						W[j]=0;
  					}
  				}

  			}
  		}
  		for (std::list<Explosion>::const_iterator iter = g.getExplosions().begin(); iter != g.getExplosions().end(); ++iter)//will it read the last one?
  		{
  			//std::cout<<" Entered Explosion Loop, location= "<<iter->x<<"\n";
  			i=std::round(iter->x);
  			for(j=i-explosionSize;j<=i+explosionSize;j++)//check if explosionSize is indeed radius
			{
				if(j>=0 && j<200)
				{
					W[j]=0;
				}
			}
   		}

   		
	}

	int Controller::pickSafeSpot(const Game& g)
	{
		int i,j;
		int mypos=round(p->x);
   		int left=0,right=0;//flag=0 implies that side of mypos is free of explosions and end is close
   		int flag;
   		
   		
   		if (W[mypos]==1)
   		{
   			flag=0;
   			j=mypos;
	   		do
	   		{
	   			j=j+1;
	   			if(j>=200)
	   			{
	   				flag=1;
	   				break;
	   			}
	   		}while(W[j]==1);
	   		
	   		if(flag==0)
	   		{
	   			right=j-mypos-1;
	   		}
	   		else
	   		{
	   			right=200-mypos-1;
	   		}

	   		flag=0;
	   		j=mypos;
	   		do
	   		{
	   			j=j-1;
	   			if(j<0)
	   			{
	   				flag=1;
	   				break;
	   			}
	   		}while(W[j]==1);
	   		
	   		if(flag==0)
	   		{
	   			left=mypos-j-1;
	   		}
	   		else
	   		{
	   			left=mypos-0;
	   		}
   		}
   		else
   		{
   			flag=0;
   			j=mypos;
   			int nosafe=0;
	   		do
	   		{
	   			j=j+1;
	   			if(j>=200)
	   			{
	   				flag=1;
	   				nosafe=1;
	   				break;
	   			}
	   		}while(W[j]==0);
	   		int firstsafe=j;
	   		do
	   		{
	   			j=j+1;
	   			if(j>=200)
	   			{
	   				flag=1;
	   				break;
	   			}
	   		}while(W[j]==1);
	   		
	   		if(flag==0)
	   		{
	   			right=j-firstsafe;
	   		}
	   		else
	   		{
	   			if(nosafe==1)
	   			{
	   				right=0;
	   			}
	   			else
	   			{
	   				right=200-firstsafe;
	   			}
	   		}

	   		flag=0;
	   		nosafe=0;
	   		j=mypos;
	   		do
	   		{
	   			j=j-1;
	   			if(j<0)
	   			{
	   				flag=1;
	   				nosafe=1;
	   				break;
	   			}
	   		}while(W[j]==0);
	   		firstsafe=j;
	   		do
	   		{
	   			j=j-1;
	   			if(j<0)
	   			{
	   				flag=1;
	   				break;
	   			}
	   		}while(W[j]==1);
	   		
	   		if(flag==0)
	   		{
	   			left=firstsafe-j;
	   		}
	   		else
	   		{
	   			if(nosafe==1)
	   			{
	   				left=0;
	   			}
	   			else
	   			{
	   				left=firstsafe-0+1;
	   			}
	   		}


   		}


   		if(left>right)
   		{
   			return(-1);
   		}
   		else if(right>left)
   		{
   			return(1);
   		}
   		else
   		{
   			//std::cout<<" Right="<<right<<" Left="<<left<<"\n";
   			return(0);
   		}
   	}

	
}


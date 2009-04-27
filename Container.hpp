#ifndef TX_CONTAINER_HPP
#define TX_CONTAINER_HPP

#include <vector>
using namespace std;

#include "Tagged.hpp"

class Contained;

typedef vector<Contained*> ChildrenVector;
typedef ChildrenVector::iterator ChildrenIterator;
/**
 * An abstract class for things that can contain or own things.
 * Useabble for maps having objects, inventories with items, etc...
 */
class Container {
	public:
		virtual void addChild(Contained* child);
		virtual void removeChild(Contained* child);
		//virtual vector<Contained*>* getChildren();
		virtual ChildrenIterator getFirstChild();
		virtual ChildrenIterator getChildEnd();

	protected:
		ChildrenVector children_;
};

/**
 * An abstract class for things that can be contained or owned by other
 * things. Useabble for objects in maps, items in inventories, etc...
 */
class Contained {
	private:
		Container* parent_;
		
	public:
		Contained(): parent_(NULL){}
		virtual void setParent(Container* parent);
		Container* getParent();
};

#endif /* TX_CONTAINER_HPP */

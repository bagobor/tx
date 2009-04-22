#include "Container.hpp"

/**
 * Adds a thing to the container.
 * @param child The thing to be contained.
 */
void Container::addChild(Contained* child) {
	if(child) {
		child->setParent(this);
		children_.push_back(child);
	}
}

/**
 * Removes a thing from the container.
 * @param child The thing to be contained.
 */
void Container::removeChild(Contained* child) {
	if(child) {
#warning ['TODO']: This...
	}
}

/**
 * Sets the parent/owner Container.
 * @param parent The thing within which this thing is contained.
 */
void Contained::setParent(Container* parent) {
	parent_ = parent;
}

/**
 * Gets the parent/owning Container of the thing.
 * @return Pointer to the parent Container.
 */
Container* Contained::getParent() {
	return parent_;
}
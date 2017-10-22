#pragma onece
#ifndef CLASS_UTILS_HPP__
#define CALSS_UTILS_HPP__
/**
*	DESCRIPTION:
*		Module contains implementation of macro utils for classes.
*	AUTHOR:
*		Mikhail Demchenko
*		mailto:dev.echo.mike@gmail.com
*		https://github.com/echo-Mike
**/

/*******************************/
/*         DESTRUCTOR          */
/*******************************/

#define VirtualDestructor(className)\
    virtual ~className();

/*******************************/
/*        CONSTRUCTOR          */
/*******************************/

#define DefaultConstructible(className)\
    className() = default;

#define NotDefaultConstructible(className)\
    className() = delete;

/*******************************/
/*            COPY             */
/*******************************/

#define DefaultCopyConstructible(className)\
    className(const className&) = default;

#define DefaultCopyAssignable(className)\
    className& operator=(const className&) = default;

#define DefaultCopyable(className)\
    DefaultCopyConstructible(className)\
    DefaultCopyAssignable(className)

#define NotCopyConstructible(className)\
    className(const className&) = delete;

#define NotCopyAssignable(className)\
    className& operator=(const className&) = delete;

#define NotCopyable(className)\
    NotCopyConstructible(className)\
    NotCopyAssignable(className)

/*******************************/
/*            MOVE             */
/*******************************/

#define DefaultMoveConstructible(className)\
    className(className&&) = default;

#define DefaultMoveAssignable(className)\
    className& operator=(className&&) = default;

#define DefaultMovable(className)\
    DefaultMoveConstructible(className)\
    DefaultMoveAssignable(className)

#define NotMoveConstructible(className)\
    className(className&&) = delete;

#define NotMoveAssignable(className)\
    className& operator=(className&&) = delete;

#define NotMovable(className)\
    NotMoveConstructible(className)\
    NotMoveAssignable(className)

/*******************************/
/*           MIXED             */
/*******************************/

#define DefaultCopyableAndMovable(className)\
    DefaultCopyable(className)\
    DefaultMovable(className)

#define NotCopyableNotMovable(className)\
    NotCopyable(className)\
    NotMovable(className)

#define DefaultCopyableNotMovable(className)\
    DefaultCopyable(className)\
    NotMovable(className)

#define DefaultMovableNotCopyable(className)\
    NotCopyable(className)\
    DefaultMovable(className)

#define DefaultCopyAndMoveConstructible(className)\
    DefaultCopyConstructible(className)\
    DefaultMoveConstructible(className)

#define DefaultCopyAndMoveAssignable(className)\
    DefaultCopyAssignable(className)\
    DefaultMoveAssignable(className)

#define DefaultConstructibleNotAssignable(className)\
    DefaultCopyAndMoveConstructible(className)\
    NotCopyAssignable(className)\
    NotMoveAssignable(className)

#define DefaultAssignableNotConstructible(className)\
    NotCopyConstructible(className)\
    NotMoveConstructible(className)\
    DefaultCopyAndMoveAssignable(className)
    
#endif
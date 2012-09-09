/*
   Copyright (C) 2012, Volker Kempert <volker.kempert@gmx.net>

   This file is part of MARMEAS application

   MARMEAS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of
   the License, or (at your option) any later version.

   MARMEAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public
   License along with MARMEAS. If not, see <http://www.gnu.org/licenses/>.

*/

#include "geometric_object_container_tests.h"
#include <QDebug>
#include "object_base.h"


CPPUNIT_TEST_SUITE_REGISTRATION (GeometricObjectContainerTests);

class GeometricObjectMock : public GeometricObject {
public:
	GeometricObjectMock() : GeometricObject() {}

};

void GeometricObjectContainerTests :: setUp (void)
{

}

void GeometricObjectContainerTests :: tearDown (void)
{
}

void GeometricObjectContainerTests::test_01_simpleCreateAndDelete (){
	qDebug() << "**GeometricObjectContainerTests::test_01_simpleCreateAndDelete**";
	GeometricObjectContainer* container =
			GeometricObjectContainerSingleton::getInstance();
	CPPUNIT_ASSERT ( container );
	delete container;
}


void GeometricObjectContainerTests::test_02_singletonCreateCreate (){
	qDebug() << "**GeometricObjectContainerTests::test_02_singletonOneFactoryCreate**";
	GeometricObjectContainer* container1 =
			GeometricObjectContainerSingleton::getInstance();
	CPPUNIT_ASSERT ( container1 );
	GeometricObjectContainer* container2 =
			GeometricObjectContainerSingleton::getInstance();
	CPPUNIT_ASSERT ( container2 );
	CPPUNIT_ASSERT ( container1 == container2 );
	delete container1;
	if ( container1 != container2) delete container2;
}

void GeometricObjectContainerTests::test_02_singletonCreateDeleteCreate (){
	qDebug() << "**GeometricObjectContainerTests::test_02_singletonTwoFactoriesCreate**";
	GeometricObjectContainer* container1 =
			GeometricObjectContainerSingleton::getInstance();
	CPPUNIT_ASSERT ( container1 );
	delete container1;
	QString *q = new QString();

	GeometricObjectContainer* container2 =
			GeometricObjectContainerSingleton::getInstance();
	delete q;
	CPPUNIT_ASSERT ( container2 );
	CPPUNIT_ASSERT ( container1 != container2 );
	delete container2;
}



void GeometricObjectContainerTests::test_03_addOneObjectAndRemove (void) {
	qDebug() << "**GeometricObjectContainerTests::test_03_addOneObjectAndRemove**";
	GeometricObjectContainer container;
	GeometricObjectMock* object_mock = new GeometricObjectMock();
	container.addObject(object_mock);
	container.removeObject(object_mock);

}

void GeometricObjectContainerTests::test_03_addManyObjectAndRemoveOne (void) {
	qDebug() << "**GeometricObjectContainerTests::test_03_addManyObjectAndRemoveOne**";
	GeometricObjectContainer container;
	GeometricObjectMock* object_mock_1 = new GeometricObjectMock();
	container.addObject(object_mock_1);
	GeometricObjectMock* object_mock_2 = new GeometricObjectMock();
	container.addObject(object_mock_2);
	GeometricObjectMock object_mock_3;
	container.addObject(&object_mock_3);

	container.removeObject(object_mock_2);
}

void GeometricObjectContainerTests::test_03_addManyObjectAndRemoveAll (void) {
	qDebug() << "**GeometricObjectContainerTests::test_03_addManyObjectAndRemoveAll**";
	GeometricObjectContainer container;
	GeometricObjectMock* object_mock_1 = new GeometricObjectMock();
	container.addObject(object_mock_1);
	GeometricObjectMock* object_mock_2 = new GeometricObjectMock();
	container.addObject(object_mock_2);

	container.removeAllObjects();

}

void GeometricObjectContainerTests::test_03_addManyObjectAndRemoveAnNoneExistingObject (void) {
	qDebug() << "**GeometricObjectContainerTests::test_03_addManyObjectAndRemoveAnNoneExistingObject**";
	GeometricObjectContainer container;
}

void GeometricObjectContainerTests::test_03_removeAnNoneExistingObject (void) {
	qDebug() << "**GeometricObjectContainerTests::test_03_removeAnNoneExistingObject**";
	GeometricObjectContainer container;
}

void GeometricObjectContainerTests::test_03_AccessASpecificObjectById (void) {
	qDebug() << "**GeometricObjectContainerTests::test_03_AccessASpecificObjectById**";
	GeometricObjectContainer container;
}

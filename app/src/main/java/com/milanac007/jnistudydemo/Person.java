package com.milanac007.jnistudydemo;

/**
 * Created by apple on 2019/10/24.
 */

public class Person {
    enum SexType{
        male,
        female,
        unkown
    }
    protected String name;
    protected SexType sex;

    Person(){

    }

    Person(String name, int sex){
        this.name = name;
        setSex(sex);
    }

    public void setSex(int sex){
        if(sex == 0){
            this.sex = SexType.male;
        }else if(sex == 1){
            this.sex = SexType.female;
        }else {
            this.sex = SexType.unkown;
        }
    }

    @Override
    public String toString() {
        return "Person{"+
                "name=" + name
                +",sex=" + sex.name()
                +"}";
    }

}

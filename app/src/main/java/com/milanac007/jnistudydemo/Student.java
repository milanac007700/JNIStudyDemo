package com.milanac007.jnistudydemo;

/**
 * Created by apple on 2019/10/22.
 */

public class Student extends Person{

    private int id;

    Student(int id, String name, int sex){
        super(name, sex);
        this.id = id;
    }


    @Override
    public String toString() {
        return "Student{"+
                "id=" + id
                +",name=" + name
                +",sex=" + sex.name()
                +"}";
    }

    private static int code = 0xff;
    protected static String getVersion(){
       return "1.0.0.1";
    }
}

package com.milanac007.jnistudydemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.Arrays;

public class MainActivity extends AppCompatActivity {
    public static final String TAG = "MainActivity";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.i(TAG, JavaLayer.getNativeInitStr());
        Log.i(TAG, "5 + 3 = " + JavaLayer.java_add(5, 3));
        Log.i(TAG, "5 - 3 = " + JavaLayer.java_sub(5, 3));
        Log.i(TAG, "6 / 3 = " + JavaLayer.java_div(6, 3));
        Log.i(TAG, "=========================================");

        int[] origArray = {1, 2, 3, 4, 5};
        Log.i(TAG, "java_addOne执行前: " + Arrays.toString(origArray));
        JavaLayer.java_addOne(origArray);
        Log.i(TAG, "java_addOne执行后: " + Arrays.toString(origArray));
        Log.i(TAG, "=========================================");

//        Log.i(TAG, "JavaLayer.java_str(\"abcdefg\"): " + JavaLayer.java_str("abcdefg"));
        Log.i(TAG, "JavaLayer.java_str(\"abcde abcde\"): " + JavaLayer.java_str("abcde abcde"));

        Log.i(TAG, "=========================================");

        String hex = "0a0b0c";
        byte[] src = JavaLayer.toByteArray(hex);
        Log.i(TAG, "java_byteArray执行前: " + Arrays.toString(src));
        byte[] dst = JavaLayer.java_byteArray(src);
        Log.i(TAG, "java_byteArray执行后: " + Arrays.toString(dst));

        Log.i(TAG, "=========================================");
        hex = "000102030405060708090a0b0c0d0e10";
        src = JavaLayer.toByteArray(hex);
        Log.i(TAG, "java_byteArray执行前: " + Arrays.toString(src));
        dst = JavaLayer.java_byteArray(src);
        Log.i(TAG, "java_byteArray执行后: " + Arrays.toString(dst));

        Log.i(TAG, "=========================================");
        Student stu = JavaLayer.java_newjobject();
        Log.i(TAG, "java_newjobject执行后: stu= " + stu);

        Log.i(TAG, "=========================================");
        Student[] students = JavaLayer.java_newjobjectArray();
        Log.i(TAG, "java_newjobjectArray执行后: students= " + Arrays.toString(students));

        Log.i(TAG, "=========================================");
        JavaLayer.java_accessStaticMember();
        try {
            Class<?> Student = Class.forName("com.milanac007.jnistudydemo.Student");
            Field codeField = Student.getDeclaredField("code");
            codeField.setAccessible(true);
            int code = codeField.getInt(null);

            Method versionMethod = Student.getDeclaredMethod("getVersion");
            String version = (String)versionMethod.invoke(null);
            Log.i(TAG,  String.format("java_accessStaticMember执行后: Student.code= 0x%02x, version=%s", code,version));
        } catch (Exception e) {
            e.printStackTrace();
        }

        Log.i(TAG, "=========================================");
        int[] srcArray = {1, 2, 3, 4, 5};
        Log.i(TAG, "java_primitiveArrayCritical执行前，srcArray= " + Arrays.toString(srcArray));
        JavaLayer.java_primitiveArrayCritical(srcArray);
        Log.i(TAG, "java_primitiveArrayCritical执行后，srcArray= " + Arrays.toString(srcArray));

        Log.i(TAG, "=========================================");
        JavaLayer.java_CallNonvirtualMethod(stu);

        Log.i(TAG, "=========================================");
        Person person = new Person();
        getPersonInfo(person);
        Log.i(TAG, "person: " + person.toString());

    }

    int getPersonInfo(Person person){
        person.name = "mike";
        person.setSex(0);
        return 0;
    }

}

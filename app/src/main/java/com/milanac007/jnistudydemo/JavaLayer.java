package com.milanac007.jnistudydemo;

public class JavaLayer {
    static {
        System.loadLibrary("native-lib");
    }

    public static native String getNativeInitStr();
    public static native int java_add(int a, int b);
    public static native int java_sub(int a, int b);


    public static native int java_div(int a, int b);

    public static native String java_str(String value);
    public static native void java_addOne(int[] values);//每个数组元素加一
    public static native byte[] java_byteArray(byte[] src);

    public static native Student java_newjobject();
    public static native Student[] java_newjobjectArray();

    public static native void java_accessStaticMember();

    public static native void java_primitiveArrayCritical(int[] src);

    public static native void java_CallNonvirtualMethod(Student instance);

        public String toHexString(byte[] byteArray) {
        if (byteArray == null) {
            return "";
        }
        return toHexString(byteArray, 0, byteArray.length);
    }
    private final char[] HEX = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    public  String toHexString(byte[] d, int s, int n) {
        if (d == null) {
            return "";
        }
        final char[] ret = new char[n * 2];
        final int e = s + n;

        int x = 0;
        for (int i = s; i < e; ++i) {
            final byte v = d[i];
            ret[x++] = HEX[0x0F & (v >> 4)];
            ret[x++] = HEX[0x0F & v];
        }
        return new String(ret);
    }

    public static byte[] toByteArray(String hexString) {
        if (null == hexString) {
            throw new IllegalArgumentException("this hexString must not be empty");
        }

        hexString = hexString.toLowerCase().replace(" ", "");
        final byte[] byteArray = new byte[hexString.length() / 2];
        int k = 0;
        for (int i = 0; i < byteArray.length; i++) {
            byte high = (byte) (Character.digit(hexString.charAt(k), 16) & 0xff);
            byte low = (byte) (Character.digit(hexString.charAt(k + 1), 16) & 0xff);
            byteArray[i] = (byte) (high << 4 | low);
            k += 2;
        }
        return byteArray;
    }

    /**
     * 把16进制字符串转换成字节数组
     * @param hex
     * @return byte[]
     */
    public byte[] hexStringToByte(String hex) {
        int len = (hex.length() / 2);
        byte[] result = new byte[len];
        char[] achar = hex.toCharArray();
        for (int i = 0; i < len; i++) {
            int pos = i * 2;
            result[i] = (byte) (toByte(achar[pos]) << 4 | toByte(achar[pos + 1]));
        }
        return result;
    }

    private int toByte(char c) {
        byte b = (byte) "0123456789ABCDEF".indexOf(c);
        return b;
    }
}

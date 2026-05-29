package com.example.jnidemo;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

/**
 * Java side of the JNI lab.
 *
 * The Activity declares native methods, loads the .so library, calls C++,
 * then displays the results on screen.
 */
public class MainActivity extends Activity {

    /*
     * "native" means:
     * this Java method has no Java body. Android will look for the implementation
     * inside the loaded native library.
     */
    public native String helloFromJNI();

    public native int factorial(int n);

    public native String reverseString(String s);

    public native int sumArray(int[] values);

    /*
     * The library name must match add_library(native-lib ...) in CMakeLists.txt.
     *
     * Do not write "libnative-lib.so" here.
     * Android automatically adds:
     * - "lib" prefix;
     * - ".so" suffix.
     */
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tvHello = findViewById(R.id.tvHello);
        TextView tvFact = findViewById(R.id.tvFact);
        TextView tvReverse = findViewById(R.id.tvReverse);
        TextView tvArray = findViewById(R.id.tvArray);
        TextView tvEdgeCases = findViewById(R.id.tvEdgeCases);

        /*
         * Demo 1:
         * Java calls a simple C++ function and receives a Java String.
         */
        tvHello.setText(section("1) helloFromJNI", helloFromJNI()));

        /*
         * Demo 2:
         * Java sends an int to C++ and receives an int result.
         */
        int fact10 = factorial(10);
        tvFact.setText(section(
                "2) factorial(10)",
                explainFactorialResult(10, fact10)
        ));

        /*
         * Demo 3:
         * Java sends a String. C++ reads it, reverses it, and returns a new String.
         */
        String sourceText = "JNI is powerful!";
        String reversed = reverseString(sourceText);
        tvReverse.setText(section(
                "3) reverseString",
                "Source : " + sourceText + "\nResult : " + reversed
        ));

        /*
         * Demo 4:
         * Java sends an int[] to native code. C++ reads the array and computes sum.
         */
        int[] numbers = {10, 20, 30, 40, 50};
        int sum = sumArray(numbers);
        tvArray.setText(section(
                "4) sumArray",
                "Input : {10, 20, 30, 40, 50}\nResult : " + sum
        ));

        /*
         * Guided edge tests:
         * These are visible directly in the app so you do not need to edit code
         * just to validate the negative/overflow/empty cases from the lab.
         */
        tvEdgeCases.setText(section(
                "5) Tests guides",
                "factorial(-5) = " + explainFactorialResult(-5, factorial(-5)) + "\n\n"
                        + "factorial(20) = " + explainFactorialResult(20, factorial(20)) + "\n\n"
                        + "reverseString(\"\") = \"" + reverseString("") + "\"\n\n"
                        + "sumArray({}) = " + sumArray(new int[]{})
        ));
    }

    private String section(String title, String body) {
        return title + "\n" + body;
    }

    private String explainFactorialResult(int n, int result) {
        if (result >= 0) {
            return "Factoriel de " + n + " = " + result;
        }

        if (result == -1) {
            return "Erreur: n negatif";
        }

        if (result == -2) {
            return "Erreur: depassement int detecte";
        }

        return "Erreur native inconnue, code = " + result;
    }
}

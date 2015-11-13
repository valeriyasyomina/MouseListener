package com.example.lera.mobilemousemanager;

import android.os.Parcelable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import java.net.Socket;
import android.view.View;
import android.view.MotionEvent;
import android.widget.Button;
import android.widget.EditText;
import android.app.AlertDialog;
import android.widget.TextView;
import java.io.DataOutputStream;
import android.view.Display;
import android.util.DisplayMetrics;
import android.view.WindowManager;
import java.io.BufferedReader;
import java.io.InputStreamReader;

public class MainActivity extends AppCompatActivity {

    private static final int BUFFER_SIZE = 32;
    private static final int MOUSE_MOVE = 7;
    private static final int MOUSE_PRESS = 1;

    private EditText editTextAddress, editTextPort;
    private Button  buttonClear;
    private TextView mainTextInfo = null;
    private Socket serverSocket = null;
    private boolean isConnected = false;
    DataOutputStream dataOutputStream = null;
    int screenWidth = 0;
    int screenHeight = 0;
    double scaleX = 0.0;
    double scaleY = 0.0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editTextAddress = (EditText)findViewById(R.id.Ip_address);
        editTextPort = (EditText)findViewById(R.id.Port);

        buttonClear = (Button)findViewById(R.id.ButtonClear);

        mainTextInfo = (TextView) findViewById(R.id.ApplicationInfoText);

        buttonClear.setOnClickListener(buttonClearClickListener);

        buttonClear.setVisibility(View.INVISIBLE);
        editTextAddress.setVisibility(View.INVISIBLE);
        editTextPort.setVisibility(View.INVISIBLE);

        WindowManager windowManager = this.getWindowManager();
        Display display = windowManager.getDefaultDisplay();
        DisplayMetrics metrics = new DisplayMetrics();
        display.getMetrics(metrics);

        screenWidth = metrics.widthPixels;
        screenHeight = metrics.heightPixels;
    }


    View.OnClickListener buttonClearClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            editTextPort.setText("");
            editTextAddress.setText("");
        }
    };

    @Override
    public  boolean onTouchEvent(MotionEvent motionEvent) {
        if (motionEvent.getAction() == MotionEvent.ACTION_DOWN) {
            if (isConnected) {
                try {
                    String mouseCommand = Integer.toString(MOUSE_PRESS) + " " + Integer.toString((int) (motionEvent.getX() * scaleX)) +
                            " " + Integer.toString((int) (motionEvent.getY() * scaleY));
                    mainTextInfo.setText(mouseCommand);
                    dataOutputStream.writeBytes(mouseCommand);
                }
                catch (Exception exception) {
                    ShowMessage("Error send mouse coordinates", exception.getMessage());
                    DisconnectThread disconnectThread = new DisconnectThread();
                    new Thread(disconnectThread).start();
                }
            }
        }
        else if (motionEvent.getAction() == MotionEvent.ACTION_MOVE) {
          //  if (isConnected) {
                try {
                   /* String mouseCommand = Integer.toString(MOUSE_MOVE) + " " + Integer.toString((int) (motionEvent.getX() * scaleX)) +
                            " " + Integer.toString((int) (motionEvent.getY() * scaleY));*/
                    String mouseCommand = Integer.toString(MOUSE_MOVE) + " " + Integer.toString((int) motionEvent.getX()) +
                            " " + Integer.toString((int)motionEvent.getY());
                    mainTextInfo.setText(mouseCommand);
                   // dataOutputStream.writeBytes(mouseCommand);
                }
                catch (Exception exception) {
                    ShowMessage("Error send mouse coordinates", exception.getMessage());
                    DisconnectThread disconnectThread = new DisconnectThread();
                    new Thread(disconnectThread).start();
                }
           // }
        }
        return true;
    }
    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
       /* outState.putBoolean("isConnected", isConnected);
        outState.putString("mainTextInfo", mainTextInfo.getText().toString());
        outState.putDouble("scaleX", scaleX);
        outState.putDouble("scaleY", scaleY);
        outState.putParcelable("serverSocket", (Parcelable) serverSocket);
        outState.putParcelable("dataOutputStream", (Parcelable) dataOutputStream);*/
    }
    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
       /* isConnected = savedInstanceState.getBoolean("isConnected");
        mainTextInfo.setText(savedInstanceState.getString("mainTextInfo"));
        scaleX = savedInstanceState.getDouble("scaleX");
        scaleY = savedInstanceState.getDouble("scaleY");
       // serverSocket = (Socket) savedInstanceState.getParcelable("serverSocket");
        //dataOutputStream = (DataOutputStream) savedInstanceState.getParcelable("dataOutputStream");*/
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        int id = item.getItemId();

        if (id == R.id.ExitAction) {
            CloseAppThread closeAppThread = new CloseAppThread();
            new Thread(closeAppThread).start();
            System.exit(0);
            return true;
        }
        else if (id == R.id.SettingsAction) {
            if (isConnected) {
                ShowMessage("Error", "To change setting disconnect first!");
            }
            else {
                buttonClear.setVisibility(View.VISIBLE);
                editTextAddress.setVisibility(View.VISIBLE);
                editTextPort.setVisibility(View.VISIBLE);
                mainTextInfo.setText(R.string.InputInvitation);
                mainTextInfo.setVisibility(View.VISIBLE);
            }
            return true;
        }
        else  if (id == R.id.DisconnectAction) {
            DisconnectThread disconnectThread = new DisconnectThread();
            new Thread(disconnectThread).start();
            return true;
        }
        else  if (id == R.id.ConnectAction) {
            ConnectThread connectThread = new ConnectThread();
            new Thread(connectThread).start();
            return  true;
        }
        else if (id == R.id.HelpAction) {
            if (isConnected) {
                ShowMessage("Error", "You must disconnect first!");
            }
            else {
                buttonClear.setVisibility(View.INVISIBLE);
                editTextAddress.setVisibility(View.INVISIBLE);
                editTextPort.setVisibility(View.INVISIBLE);
                mainTextInfo.setText(R.string.HelpInfo);
                mainTextInfo.setVisibility(View.VISIBLE);
            }
            return  true;
        }
        return super.onOptionsItemSelected(item);
    }


    class ConnectThread implements Runnable {
        public void run() {
            try {
                if (isConnected) {
                    ShowMessage("Connection error", "You have already been connected!");
                }
                else {
                    serverSocket = new Socket(editTextAddress.getText().toString(),
                            Integer.parseInt(editTextPort.getText().toString()));

                    BufferedReader inputStream = new BufferedReader(new InputStreamReader(serverSocket.getInputStream()));

                    char[] buffer = new char[BUFFER_SIZE];
                    inputStream.read(buffer, 0, BUFFER_SIZE);
                    dataOutputStream = new DataOutputStream(serverSocket.getOutputStream());

                    String PCScreenSize = String.valueOf(buffer);

                    String[] screenSize = PCScreenSize.split(" ");
                    scaleX = (double) Double.parseDouble(screenSize[0]) / screenWidth;
                    scaleY = (double) Double.parseDouble(screenSize[1]) / screenHeight;
                    ShowMessage("Connection ok", "Connection successfully established!");
                    HideAllComponents();
                    isConnected = true;
                }
            }
            catch (Exception exception) {
                exception.printStackTrace();
                ShowMessage("Connection error", exception.getMessage());
                ShowMainAppInfo();
            }
        }
    }

    class DisconnectThread implements  Runnable {
        public void run() {
            try {
                if (!isConnected) {
                    ShowMessage("Disconnect error", "You have already been disconnected!");
                }
                else {
                    serverSocket.close();
                    isConnected = false;
                    ShowMessage("Disconnect ok", "You have been successfully disconnected!");
                    ShowMainAppInfo();
                }
            }
            catch (Exception exception) {
                exception.printStackTrace();
                ShowMessage("Disconnect error", exception.getMessage());
                ShowMainAppInfo();
            }
        }
    }

    class CloseAppThread implements  Runnable {
        public void run() {
            try {
                if (isConnected) {
                    serverSocket.close();
                }
            }
            catch (Exception exception) {
            }
        }
    }

    public  void HideAllComponents() {
        MainActivity.this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                buttonClear.setVisibility(View.INVISIBLE);
                editTextAddress.setVisibility(View.INVISIBLE);
                editTextPort.setVisibility(View.INVISIBLE);
                mainTextInfo.setVisibility(View.VISIBLE); //invisible
            }
        });
    }
    public  void ShowMainAppInfo() {
        MainActivity.this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                buttonClear.setVisibility(View.INVISIBLE);
                editTextAddress.setVisibility(View.INVISIBLE);
                editTextPort.setVisibility(View.INVISIBLE);
                mainTextInfo.setText(R.string.ApplicationInfo);
                mainTextInfo.setVisibility(View.VISIBLE);
            }
        });
    }

    public void ShowMessage(final String title, final String message) {
        MainActivity.this.runOnUiThread(new Runnable() {
            public void run() {
                AlertDialog.Builder dlgAlert  = new AlertDialog.Builder(MainActivity.this);
                dlgAlert.setMessage(message);
                dlgAlert.setTitle(title);
                dlgAlert.setPositiveButton("OK", null);
                dlgAlert.setCancelable(true);
                dlgAlert.create().show();
            }
        });
    }
}



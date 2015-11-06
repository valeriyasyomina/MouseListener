package com.example.lera.mobilemousemanager;

import android.app.Activity;
import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import android.os.AsyncTask;
import android.view.View;
import android.view.MotionEvent;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.app.AlertDialog;
import android.widget.TextView;


public class MainActivity extends AppCompatActivity {

    private TextView textResponse;
    private EditText editTextAddress, editTextPort;
    private Button buttonConnect, buttonClear;
    private Socket serverSocket = null;
    private boolean isConnected = false;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editTextAddress = (EditText)findViewById(R.id.Ip_address);
        editTextPort = (EditText)findViewById(R.id.Port);
        buttonConnect = (Button)findViewById(R.id.ButtonConnect);
        buttonClear = (Button)findViewById(R.id.ButtonClear);
        textResponse = (TextView)findViewById(R.id.InfoText);
        buttonConnect.setOnClickListener(buttonConnectOnClickListener);
    }

    OnClickListener buttonConnectOnClickListener =  new OnClickListener(){
                @Override
                public void onClick(View arg0) {
                    ConnectThread connectThread = new ConnectThread();
                    new Thread(connectThread).start();
                }
    };



    @Override
    public  boolean onTouchEvent(MotionEvent motionEvent) {
        if (motionEvent.getAction() == MotionEvent.ACTION_DOWN) {
            textResponse.setText("Down");
            AlertDialog.Builder dlgAlert  = new AlertDialog.Builder(this);
            dlgAlert.setMessage("This is an alert with no consequence");
            dlgAlert.setTitle("App Title");
            dlgAlert.setPositiveButton("OK", null);
            dlgAlert.setCancelable(true);
            dlgAlert.create().show();
        }
        else  if (motionEvent.getAction() == MotionEvent.ACTION_MOVE) {
            if (isConnected) {
                float x = motionEvent.getX();
                float y = motionEvent.getY();
                textResponse.setText(Float.toString(x) + " " + Float.toString(y));
            }
        }
        return true;
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_logout) {
            AlertDialog.Builder dlgAlert  = new AlertDialog.Builder(this);
            dlgAlert.setMessage("logout");
            dlgAlert.setTitle("App Title");
            dlgAlert.setPositiveButton("OK", null);
            dlgAlert.setCancelable(true);
            dlgAlert.create().show();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }


    class ConnectThread implements Runnable {



        public void run() {
         //   Socket socket = null;
            try {
                serverSocket = new Socket(editTextAddress.getText().toString(),
                        Integer.parseInt(editTextPort.getText().toString()));
                showMessage("Connection ok", "Connection successfully established!");
                isConnected = true;
            }
            catch (IOException exception) {
                exception.printStackTrace();
                showMessage("Connection error", exception.getMessage());
            }
        }
    }

    public void showMessage(final String title, final String message) {
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


/*public class MyClientTask extends AsyncTask<Void, Void, Void> {

    String dstAddress;
    int dstPort;
    String response = "";

    MyClientTask(String addr, int port) {
        dstAddress = addr;
        dstPort = port;
    }
    @Override
    protected Void doInBackground(Void... arg0) {

        Socket socket = null;
        return  null;
    }
}*/

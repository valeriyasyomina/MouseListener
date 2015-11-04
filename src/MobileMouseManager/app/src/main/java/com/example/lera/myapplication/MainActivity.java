package com.example.lera.myapplication;

import android.support.v7.app.AppCompatActivity;

import android.os.Bundle;
import java.io.IOException;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import java.net.Socket;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.EditText;
import java.net.InetAddress;

public class MainActivity extends AppCompatActivity {

    private Button connectButton;
    private Button clearButton;
    private EditText serverIpAddressView;
    private EditText portNumberView;

    private int portNumber;
    private String serverIpAddress = "";

    private TextView textInfo;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        serverIpAddressView = (EditText) this.findViewById(R.id.Ip_address);
        portNumberView = (EditText) this.findViewById(R.id.Port);
        connectButton = (Button) this.findViewById(R.id.ButtonConnect);
        clearButton = (Button) this.findViewById(R.id.ButtonClear);

        connectButton.setOnClickListener(buttonConnectOnCLickListener);
        clearButton.setOnClickListener(buttonClearOnCLickListener);

        textInfo = (TextView) this.findViewById(R.id.InfoText);
    }

    OnClickListener buttonConnectOnCLickListener = new OnClickListener() {
        @Override
        public void onClick(View v) {
            serverIpAddress = serverIpAddressView.getText().toString();
            portNumber = Integer.parseInt(portNumberView.getText().toString());
            connectThread.start();
        }
    };

    OnClickListener buttonClearOnCLickListener = new OnClickListener() {
        @Override
        public void onClick(View v) {
            serverIpAddressView.setText("");
            portNumberView.setText("");
        }
    };

    Thread connectThread = new Thread(new Runnable() {
        @Override
        public void run() {
            Socket socket = null;

            try{
                socket = new Socket(serverIpAddress,portNumber);
                socket.close();
            }
            catch (Exception ex){
                ex.printStackTrace();
            }


        }
    });




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
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }


   /* public void ConnectToServer(View v){

        String serverIpAddress = serverIpAddressView.getText().toString();
        int portNumber = Integer.parseInt(portNumberView.getText().toString());
        Socket socket = new Socket(serverIpAddress, portNumber);


        socket.close();

    }*/

}

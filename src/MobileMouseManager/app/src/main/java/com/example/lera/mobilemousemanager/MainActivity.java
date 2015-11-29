package com.example.lera.mobilemousemanager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import java.net.InetAddress;
import java.net.Socket;
import android.view.View;
import android.view.MotionEvent;
import android.widget.Button;
import android.app.AlertDialog;
import android.widget.TextView;
import android.view.Display;
import android.util.DisplayMetrics;
import android.view.WindowManager;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.DatagramSocket;
import java.net.DatagramPacket;
import java.net.SocketException;
import java.net.SocketTimeoutException;

public class MainActivity extends AppCompatActivity {

    private static final int BUFFER_SIZE = 64;
    private static final int MOUSE_MOVE = 7;
    private static final int MOUSE_LEFT_BUTTON_PRESS = 1;
    private static final int MOUSE_RIGHT_BUTTON_PRESS = 2;
    private static final int UDP_PORT_INDEX = 0;
    private static final int SCREEN_WIDTH_INDEX = 1;
    private static final int SCREEN_HEIGHT_INDEX = 2;
    private static final int BROADCAST_PORT_NUMBER = 9000;
    private static final int RECEIVE_INTERVAL_MILE_SECONDS = 1000;
    private static final String BROADCAST_IP_ADDRESS = "255.255.255.255";

    private Button leftMouseButton, rightMouseButton;
    private TextView mainTextInfo = null;
    private Socket serverSocket = null;
    private boolean isConnected = false;

    int screenWidth = 0, screenHeight = 0;
    double scaleX = 0.0, scaleY = 0.0;
    int mouseX = 0, mouseY = 0;

    private int UDPPortNumber = 0;
    private int TCPPortNumber = 0;

    private DatagramSocket serverDatagramSocket = null;
    private InetAddress serverAddress = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        leftMouseButton = (Button) findViewById(R.id.LeftMouseButton);
        rightMouseButton = (Button) findViewById(R.id.RightMouseButton);

        mainTextInfo = (TextView) findViewById(R.id.ApplicationInfoText);

        leftMouseButton.setOnClickListener(leftMouseButtonOnClickListener);
        rightMouseButton.setOnClickListener(rightMouseButtonOnClickListener);

        leftMouseButton.setVisibility(View.INVISIBLE);
        rightMouseButton.setVisibility(View.INVISIBLE);

        WindowManager windowManager = this.getWindowManager();
        Display display = windowManager.getDefaultDisplay();
        DisplayMetrics metrics = new DisplayMetrics();
        display.getMetrics(metrics);

        screenWidth = metrics.widthPixels;
        screenHeight = metrics.heightPixels;

        FindServerThread findServerThread = new FindServerThread();
        new Thread(findServerThread).start();
    }

    private void FindServer() {
        try {

            DatagramSocket findServerSocket = new DatagramSocket();
            findServerSocket.setBroadcast(true);

            byte[] serverMessage = new byte[BUFFER_SIZE];
            DatagramPacket serverAnswer = new DatagramPacket(serverMessage, serverMessage.length);

            String message = "New client";
            DatagramPacket datagramPacket = new DatagramPacket(message.getBytes(), message.length(),
                    InetAddress.getByName(BROADCAST_IP_ADDRESS), BROADCAST_PORT_NUMBER);

            findServerSocket.send(datagramPacket);
            findServerSocket.setSoTimeout(RECEIVE_INTERVAL_MILE_SECONDS);
            findServerSocket.receive(serverAnswer);

            serverAddress = serverAnswer.getAddress();

            String portNumberStr = new String(serverAnswer.getData());
            portNumberStr = portNumberStr.trim().replaceAll("\n", "");
            TCPPortNumber = Integer.parseInt(portNumberStr);

            findServerSocket.close();

        } catch (Exception exception) {
            if (exception instanceof SocketException)
                ShowMessage("Network exception", exception.getMessage());
            else if (exception instanceof SocketTimeoutException)
                ShowMessage("Find server exception", "Server not found! Launch server and restart mobile app");
            else
                ShowMessage("Start app exception", exception.getMessage());
        }
    };

    View.OnClickListener leftMouseButtonOnClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            SendMouseCommandThread sendCommandThread = new SendMouseCommandThread(MOUSE_LEFT_BUTTON_PRESS);
            new Thread(sendCommandThread).start();
        }
    };

    View.OnClickListener rightMouseButtonOnClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            SendMouseCommandThread sendCommandThread = new SendMouseCommandThread(MOUSE_RIGHT_BUTTON_PRESS);
            new Thread(sendCommandThread).start();
        }
    };

    @Override
    public  boolean onTouchEvent(MotionEvent motionEvent) {
        mouseX = (int) motionEvent.getX();
        mouseY = (int) motionEvent.getY();
        SendMouseCommandThread sendCommandThread = new SendMouseCommandThread(MOUSE_MOVE);
        new Thread(sendCommandThread).start();
        return true;
    }
    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putString("mainTextInfo", mainTextInfo.getText().toString());

        outState.putBoolean("WasConnected", false);

        if (isConnected) {
            DisconnectThread disconnectThread = new DisconnectThread();
            new Thread(disconnectThread).start();
            outState.putBoolean("WasConnected", true);
        }
    }
    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
        mainTextInfo.setText(savedInstanceState.getString("mainTextInfo"));
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
        }else if (id == R.id.DisconnectAction) {
            DisconnectThread disconnectThread = new DisconnectThread();
            new Thread(disconnectThread).start();
            return true;
        } else if (id == R.id.ConnectAction) {
            ConnectThread connectThread = new ConnectThread();
            new Thread(connectThread).start();
            return true;
        } else if (id == R.id.HelpAction) {
            if (isConnected) {
                ShowMessage("Error", "You must disconnect first!");
            } else {
                leftMouseButton.setVisibility(View.INVISIBLE);
                rightMouseButton.setVisibility(View.INVISIBLE);
                mainTextInfo.setText(R.string.HelpInfo);
                mainTextInfo.setVisibility(View.VISIBLE);
            }
            return true;
        }
        return super.onOptionsItemSelected(item);
    }


    class ConnectThread implements Runnable {
        public void run() {
            try {
                if (isConnected) {
                    ShowMessage("Connection error", "You have already been connected!");
                } else {

                    serverSocket = new Socket(serverAddress, TCPPortNumber);

                    BufferedReader inputStream = new BufferedReader(new InputStreamReader(serverSocket.getInputStream()));

                    char[] buffer = new char[BUFFER_SIZE];
                    inputStream.read(buffer, 0, BUFFER_SIZE);

                    String serverData = String.valueOf(buffer);

                    String[] stringsArray = serverData.split(" ");
                    UDPPortNumber = Integer.parseInt(stringsArray[UDP_PORT_INDEX]);
                    scaleX = (double) Double.parseDouble(stringsArray[SCREEN_WIDTH_INDEX]) / screenWidth;
                    scaleY = (double) Double.parseDouble(stringsArray[SCREEN_HEIGHT_INDEX]) / screenHeight;

                    serverDatagramSocket = new DatagramSocket();
                    serverDatagramSocket.setBroadcast(false);

                    ShowMessage("Connection ok", "Connection successfully established!");
                    ShowOnlyControlComponents();
                    isConnected = true;
                }
            } catch (Exception exception) {
                exception.printStackTrace();
                ShowMessage("Connection error", exception.getMessage());
                ShowMainAppInfo();
            }
        }
    }

    class SendMouseCommandThread implements Runnable {
        private int mouseCommandType;
        public  SendMouseCommandThread(int mouseCommandType) {
            this.mouseCommandType = mouseCommandType;
        }
        public void run() {
            if (isConnected) {
                try {
                    String mouseCommand = Integer.toString(mouseCommandType) + " " +
                            Integer.toString((int) (mouseX * scaleX)) + " " +
                            Integer.toString((int) (mouseY * scaleY));

                    DatagramPacket datagramPacket = new DatagramPacket(mouseCommand.getBytes(),
                            mouseCommand.length(), serverAddress, UDPPortNumber);

                    serverDatagramSocket.send(datagramPacket);

                } catch (Exception exception) {
                    ShowMessage("Error send mouse coordinates", exception.getMessage());
                    DisconnectThread disconnectThread = new DisconnectThread();
                    new Thread(disconnectThread).start();
                }
            }
        }
    }

    class FindServerThread implements Runnable {
        public void run() {
            FindServer();
        }
    }

    class DisconnectThread implements  Runnable {
        public void run() {
            try {
                if (!isConnected) {
                    ShowMessage("Disconnect error", "You have already been disconnected!");
                } else {
                    serverSocket.close();
                    serverDatagramSocket.close();
                    isConnected = false;
                    ShowMessage("Disconnect ok", "You have been successfully disconnected!");
                    ShowMainAppInfo();
                }
            } catch (Exception exception) {
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
                    serverDatagramSocket.close();
                }
            } catch (Exception exception) {
            }
        }
    }

    public void ShowOnlyControlComponents() {
        MainActivity.this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mainTextInfo.setVisibility(View.INVISIBLE);
                leftMouseButton.setVisibility(View.VISIBLE);
                rightMouseButton.setVisibility(View.VISIBLE);
            }
        });
    }
    public void ShowMainAppInfo() {
        MainActivity.this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                leftMouseButton.setVisibility(View.INVISIBLE);
                rightMouseButton.setVisibility(View.INVISIBLE);
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



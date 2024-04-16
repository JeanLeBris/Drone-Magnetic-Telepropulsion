package com.isen.esp_joystick;

import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import com.github.angads25.toggle.widget.LabeledSwitch;

import java.io.IOException;
import java.io.OutputStream;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private static final int REQUEST_BT_DEVICE = 1;

    private BluetoothAdapter bluetoothAdapter;
    private BluetoothSocket bluetoothSocket;
    private OutputStream outputStream;
    private boolean isConnected = false;

    private View joystick;
    private View borderCircle;
    private ImageView champB;
    private ConstraintLayout parentLayout;
    private TextView Xtxt;
    private TextView Ytxt;
    private float positionX=0;
    private float positionY=0;
    private float borderCircle_Radius;

    private Handler handler = new Handler();
    private final int DELAY_INTERVAL = 5; // Interval en millisecondes

    private Dialog customDialog;


    private int sens = -1;
    private TextView textBt;
    private String macAddress;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Créer la boîte de dialogue
        customDialog = new Dialog(MainActivity.this, R.style.FullScreenDialog);
        customDialog.setContentView(R.layout.bt_connection);

        // Lancer l'activité Bluetooth
        Intent bluetoothIntent = new Intent(MainActivity.this, BluetoothActivity.class);
        startActivityForResult(bluetoothIntent,REQUEST_BT_DEVICE);

        joystick = findViewById(R.id.joystick);
        parentLayout = findViewById(R.id.layout_main);
        borderCircle = findViewById(R.id.background_circle);
        Xtxt = findViewById(R.id.textViewX);
        Ytxt = findViewById(R.id.textViewY);
        champB = findViewById(R.id.champB);
        textBt = findViewById(R.id.textView4);

        joystick.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        // Convertir dp en pixels
                        int widthInDp = 50; // Nouvelle largeur en dp
                        int heightInDp = 50; // Nouvelle hauteur en dp
                        float density = getResources().getDisplayMetrics().density;
                        int widthInPixels = (int) (widthInDp * density);
                        int heightInPixels = (int) (heightInDp * density);

                        // Créer de nouveaux paramètres de mise en page avec la nouvelle taille
                        ViewGroup.LayoutParams params = joystick.getLayoutParams(); // Récupérer les paramètres de mise en page existants
                        params.width = widthInPixels; // Définir la nouvelle largeur en pixels
                        params.height = heightInPixels; // Définir la nouvelle hauteur en pixels

                        // Appliquer les nouveaux paramètres de mise en page à la vue
                        joystick.setLayoutParams(params);
                    case MotionEvent.ACTION_MOVE:

                        borderCircle_Radius = borderCircle.getWidth() / 2;

                        // Coordonnées du centre de border_circle par rapport à l'écran
                        int[] borderCircleLocation = new int[2];
                        borderCircle.getLocationOnScreen(borderCircleLocation);
                        //float centreX = borderCircleLocation[0] + borderCircle.getWidth() / 2;
                        //float centreY = borderCircleLocation[1] + joystick.getHeight();
                        float centreX = borderCircleLocation[0] + borderCircle.getWidth() / 2;
                        float centreY = borderCircleLocation[1] + joystick.getHeight()*2;

                        // Coordonnées relatives du doigt par rapport à la vue parente
                        float touchX = event.getX() + v.getX();
                        float touchY = event.getY() + v.getY();

                        // Calculer la distance entre le centre du cercle de bord et la position actuelle du joystick
                        float dx = touchX - centreX;
                        float dy = touchY - centreY;
                        float distanceToCenter = (float) Math.sqrt(dx * dx + dy * dy);

                        // Si le joystick est en dehors du cercle de bord, ajuster les coordonnées
                        if (distanceToCenter > borderCircle_Radius) {
                            // Calculer l'angle du joystick par rapport au centre du cercle de bord
                            float angle = (float) Math.atan2(dy, dx);

                            // Calculer les nouvelles coordonnées en limitant la distance au rayon du cercle de bord
                            float newX = centreX + borderCircle_Radius * (float) Math.cos(angle);
                            float newY = centreY + borderCircle_Radius * (float) Math.sin(angle);

                            // Déplacer le joystick aux nouvelles coordonnées en tenant compte du décalage
                            joystick.setX(newX - joystick.getWidth() / 2);
                            joystick.setY(newY - joystick.getHeight() / 2);
                        } else {
                            // Si le joystick est à l'intérieur du cercle de bord, le déplacer directement
                            joystick.setX(touchX - joystick.getWidth() / 2);
                            joystick.setY(touchY - joystick.getHeight() / 2);
                        }


                        // Calculer la position du joystick par rapport au centre du cercle de bord
                        float offsetX = joystick.getX() + joystick.getWidth() / 2 - centreX;
                        float offsetY = joystick.getY() + joystick.getHeight() / 2 - centreY;

                        // Convertir en pourcentage par rapport à la moitié de la largeur/hauteur de border_circle
                        positionY = -(offsetX / (borderCircle.getWidth() / 2)) * 100;
                        positionX = sens*(offsetY / (borderCircle.getHeight() / 2)) * 100;

                        // Limiter les valeurs entre -100 et 100
                        positionX = Math.max(-100, Math.min(100, positionX));
                        positionY = Math.max(-100, Math.min(100, positionY));

                        Xtxt.setText("X: "+ (int) positionX);
                        Ytxt.setText("Y: "+ (int) positionY);


                        Log.d("TAG", "positionX: " + (int) positionX + " positionY: " + (int) positionY);


                        // Calcul de l'angle du joystick
                        float joystickAngle = (float) Math.toDegrees(Math.atan2(offsetY, offsetX));

                        // Appel de la fonction pour faire pivoter l'image
                        champB.setVisibility(View.VISIBLE);

                        champB.setAlpha(map(distanceToCenter, 0, borderCircle_Radius, 0F, 0.8F));
                        setRedCanal(map(distanceToCenter, 0, borderCircle_Radius, 0.1F, 1F)*100);
                        rotateImg(champB, joystickAngle-90);
                        return true;

                    case MotionEvent.ACTION_UP:
                        // Ramener le joystick au centre
                        int centerX = (parentLayout.getWidth() - joystick.getWidth()) / 2;
                        int centerY = (parentLayout.getHeight() - joystick.getHeight()) / 2;
                        joystick.setX(centerX);
                        joystick.setY(centerY);

                        positionX = 0;
                        positionY = 0;
                        if(isConnected) {
                            sendData((int) positionX, (int) positionY);
                        }
                        Xtxt.setText("X: "+ (int) positionX);
                        Ytxt.setText("Y: "+ (int) positionY);

                        // Convertir dp en pixels
                        widthInDp = 35; // Nouvelle largeur en dp
                        heightInDp = 35; // Nouvelle hauteur en dp
                        density = getResources().getDisplayMetrics().density;
                        widthInPixels = (int) (widthInDp * density);
                        heightInPixels = (int) (heightInDp * density);

                        // Créer de nouveaux paramètres de mise en page avec la nouvelle taille
                        ViewGroup.LayoutParams params2 = joystick.getLayoutParams(); // Récupérer les paramètres de mise en page existants
                        params2.width = widthInPixels; // Définir la nouvelle largeur en pixels
                        params2.height = heightInPixels; // Définir la nouvelle hauteur en pixels

                        // Appliquer les nouveaux paramètres de mise en page à la vue
                        joystick.setLayoutParams(params2);

                        champB.setVisibility(View.INVISIBLE);
                        return true;

                    default:
                        return false;
                }
            }
        });

        LabeledSwitch sensSwitch = findViewById(R.id.sensSwitch);
        ImageView systemeViewUp = findViewById(R.id.systemeUp);
        ImageView systemeViewDown = findViewById(R.id.systemeDown);

        sensSwitch.setOnToggledListener((toggleableView, isOn) -> {
            // Handle the Switch state change here
            if (isOn) {
                systemeViewUp.setVisibility(View.VISIBLE);
                systemeViewDown.setVisibility(View.INVISIBLE);
                sens = 1;


            } else {

                systemeViewUp.setVisibility(View.INVISIBLE);
                systemeViewDown.setVisibility(View.VISIBLE);
                sens = -1;

            }
        });



        // Initialiser le Bluetooth adapter
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth not supported", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }




    }
    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_BT_DEVICE) {
            if (resultCode == RESULT_OK && data != null) {
                macAddress = data.getStringExtra("MAC_ADDRESS");
                // Faites ce que vous voulez avec l'adresse MAC ici
                // Connect to Bluetooth device
                showPopup();
                connectToBluetoothDevice(macAddress);
            } else {
                // Gérer l'annulation ou l'erreur

            }
        }
    }



    public static float map(float x, float inMin, float inMax, float outMin, float outMax) {
        return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }

    // Fonction pour faire pivoter l'image d'un angle donné
    private void rotateImg(ImageView imageView, float angle) {
        imageView.setRotation(angle);
    }
    // Fonction pour augementer au non la valeur du canal rouge d'une image ( utiliser pour rendre plus ou moin rouge l'image de simulation de champ magnétique)
    public void setRedCanal(float valPercentage) {
        // Assurez-vous que la valeur est dans la plage 0-100
        valPercentage = Math.max(0, Math.min(100, valPercentage));

        // Calculez le coefficient pourcentage en fonction de la valeur spécifiée
        float redCoefficient = valPercentage / 100.0f;

        // Créez une matrice de couleur personnalisée
        ColorMatrix colorMatrix = new ColorMatrix(new float[] {
                redCoefficient, 0, 0, 0, 0,    // Red
                0, 1, 0, 0, 0,                  // Green
                0, 0, 1, 0, 0,                  // Blue
                0, 0, 0, 1, 0                   // Alpha
        });

        // Créez un filtre ColorMatrixColorFilter avec la matrice de couleur personnalisée
        ColorMatrixColorFilter colorFilter = new ColorMatrixColorFilter(colorMatrix);

        // Appliquer le filtre à l'ImageView (champB)
        champB.setColorFilter(colorFilter);
    }


    private void connectToBluetoothDevice(String address) {
        Log.d("TAG", "connectToBluetoothDevice: ");
        BluetoothDevice device = bluetoothAdapter.getRemoteDevice(address);
        UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"); // Standard UUID for SPP
        try {

            bluetoothSocket = device.createRfcommSocketToServiceRecord(uuid);
            bluetoothSocket.connect();
            outputStream = bluetoothSocket.getOutputStream();
            dismissPopup();
            textBt.setText("Connected");
            isConnected = true;
            Toast.makeText(this, "Connected", Toast.LENGTH_SHORT).show();
            startSendingData();
        } catch (IOException e) {
            Toast.makeText(this, "Failed to connect", Toast.LENGTH_SHORT).show();
            tryReconnect();
            e.printStackTrace();
        }
    }

    // Fonction pour envoyer les données vers l'ESP 32
    private void sendData(int x, int y) {
        if(isConnected) {
            try {
                String data = x + "," + y + "\n";
                outputStream.write(data.getBytes());
            } catch (IOException e) {
                isConnected = false;
                Toast.makeText(this, "Failed to send data", Toast.LENGTH_SHORT).show();
                e.printStackTrace();
            }
        }
        else{
            if (bluetoothSocket != null) {
                try {
                    bluetoothSocket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            textBt.setText("Disonnected");
            Log.d("TAG", "sendData: Bt disconnected");
            stopSendingData();
            showPopup();
            tryReconnect();
        }

    }

    // Méthode pour tenter de se reconnecter à l'appareil Bluetooth à intervalles de 5 secondes
    private void tryReconnect() {
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                Log.d("TAG", "run: tryreconnect");
                if (!isConnected) { // Si la connexion n'est pas établie
                    connectToBluetoothDevice(macAddress); // Tente de se reconnecter
                    handler.postDelayed(this, DELAY_INTERVAL); // Planifie la prochaine tentative après 5 secondes
                }
            }
        }, DELAY_INTERVAL);
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        stopSendingData();
        if (bluetoothSocket != null) {
            try {
                bluetoothSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }



    // Méthode pour démarrer l'envoi de données
    private void startSendingData() {
        handler.postDelayed(sendDataRunnable, DELAY_INTERVAL);
    }

    // Méthode pour arrêter l'envoi de données
    private void stopSendingData() {
        handler.removeCallbacks(sendDataRunnable);
    }

    // Runnable pour envoyer les données à intervalles réguliers
    private Runnable sendDataRunnable = new Runnable() {
        @Override
        public void run() {
            sendData((int) positionX, (int) positionY); // Envoyer les données
            handler.postDelayed(this, DELAY_INTERVAL); // Programmer la prochaine exécution
        }
    };

    //Fonction pour afficher une popup personaliser
   private void showPopup() {
       if (customDialog != null && !customDialog.isShowing()) {

           // Afficher la boîte de dialogue
           customDialog.show();
       }

    }
    //Fonction pour enlever la popup
    private void dismissPopup() {
        if (customDialog != null && customDialog.isShowing()) {
            customDialog.dismiss();
        }
    }



}

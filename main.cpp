#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include<math.h>

struct Header1
{
    uint16_t rsv:3;
    uint16_t upper_level_device_id:10;
    uint16_t rsv2:3;
};
struct Header2
{   
    uint16_t device_type_NL:4;
    uint16_t info:8;
    uint16_t device_type_NH:4;
};
struct Header3
{
    uint16_t lower_level_devices_count:6;
    uint16_t ID:10;
};


/**
 * @brief Funcion extract16
 * 
 * Esta funcion sirve para extraer los bits de tipo 16.
 * @param num_bits  Número de bits a extraer
 * @param mask  Máscara para extraer los bits
 * @returns (data >> inicio) & mask;  Desplaza a la derecha y aplica máscara
 * 
 */
//Funcion para extraer bit de 16 bits 
uint16_t extract16(uint16_t data, uint32_t inicio, uint32_t final) {
    uint32_t num_bits = final - inicio + 1;
    uint16_t mask = (1U << num_bits) - 1;
    return (data >> inicio) & mask;
}

/**
 * @brief Funcion extractHeader1
 * 
 * Funcion para extraer los datos del mapa de bits haciendo uso tambien de extract16.
 * @param head  Renombramos a nuestra estructura en la funcion.
 * @param encabezadoData  data en el cual se usa la funcion extract16 para ir al bit a usar.
 * @returns head (Para usar obviamente en el main)
 * 
 */
//Funcion para extraer cada bits del header


Header1 extractHeader1(uint16_t encabezadoData) {
    Header1 head;
    head.upper_level_device_id = extract16(encabezadoData, 3, 12);
    return head;
}
/**
 * @brief Funcion extractHeader2
 * 
 * Funcion para extraer los datos del mapa de bits haciendo uso tambien de extract16.
 * @param head  Renombramos a nuestra estructura en la funcion.
 * @param encabezadoData  data en el cual se usa la funcion extract16 para ir al bit a usar.
 * @returns head (Para usar obviamente en el main)
 * 
 */
//Funcion para extraer cada bits del header
Header2 extractHeader2(uint16_t encabezadoData) 
{
    Header2 head;
    head.device_type_NL=extract16(encabezadoData,0,3);
    head.info = extract16(encabezadoData, 4, 11);
    head.device_type_NH=extract16(encabezadoData,12,15);
    return head;
}
/**
 * @brief Funcion extractHeader3
 * 
 * Funcion para extraer los datos del mapa de bits haciendo uso tambien de extract16.
 * @param head  Renombramos a nuestra estructura en la funcion.
 * @param encabezadoData  data en el cual se usa la funcion extract16 para ir al bit a usar.
 * @returns head (Para usar obviamente en el main)
 * 
 */
//Funcion para extraer cada bits del header
Header3 extractHeader3(uint16_t encabezadoData) 
{
    Header3 head;
    head.lower_level_devices_count = extract16(encabezadoData, 0, 5);
    head.ID = extract16(encabezadoData,6, 15);
    return head;
}

/**
 * @brief Funcion show sin uso en el main creada en un principio para mostrar los datos 
 * 
 * @param head para utilizar el header a trabajar
 */
void showCampo2(struct Header2 head) 
{
    int count;
    for (int i=0;i<count;i++) 
    {
        printf("--------------\n");
        if(head.device_type_NL==0 && head.device_type_NH==0)
        {
            printf("CPU");
        }
        if (head.device_type_NL==1 && head.device_type_NH==0)
        {
            printf("SENSOR: ");
            if (head.info==0)
                printf("DEL TIPO FLOW");
            else if (head.info==1)
                printf("DEL TIPO TEMP");
            else if (head.info==2)
                printf("DEL TIPO PRESION");
            else if (head.info==3)
                printf("DEL TIPO NIVEL");   
        }
        if (head.device_type_NL==0 && head.device_type_NH==1)
        {
            printf("ACTUADOR: ");
            if (head.info==0)
                printf("DEL TIPO VALVULA");
            else if(head.info==1)
                printf("DEL TIPO MOTOR");    
        }
        if (head.device_type_NL==1 && head.device_type_NH==1)
        {
            printf("CONCENTRATOR: ");
        }       
    }
}
/**
 * @brief Funcion showConnectionSequence 
 * 
 * Fucnion para mostrar la secuencia de conexion de un dispositivo
 * @param head Para utilizar el header a trabajar
 * @param id ID Ingresado por el usuario para saber su secuencia 
 * @param break Salimos del bucle ya que hemos encontrado el ID(linea- 155)
 * @param bool found "Variable tipo bandera inicializada en false"
 * @param true Para saber que encontramos el ID Marcamos found como true
 */
// Función para mostrar la secuencia de conexión de un dispositivo
void showConnectionSequence(struct Header1 head, uint16_t id) {
    while (id != 0xFFFF) { // 0xFFFF indica que no hay conexión superior
        printf("ID: %u\n", id);
        bool found = false;
        int count;
        for (int i=0;i<count;i++) 
        {
            if (head.upper_level_device_id == id) 
            {
                id = head.upper_level_device_id;
                found = true; // Hemos encontrado el ID, así que marcamos found como true
                break; // Salimos del bucle ya que hemos encontrado el ID
            }
        }
        if (!found) break; // Si no se encuentra el ID, se termina la secuencia
    }
}
/**
 * @brief Funcion devices_types
 * 
 * Funcion para trabajar los sensores y actuadores en con sus respectivos condicionales
 * @param device_type1 device_type_NL
 * @param device_type2 device_type_NH
 * @param info Informacion del tipo a trabajar
 * 
 * @return info
 */
uint16_t device_types(uint16_t device_type1,uint16_t device_type2,uint16_t info)
{
    //struct Header2 head;
    if (device_type1==0&&device_type2==0)
    {
        printf("CPU");
    }
    if (device_type1==1 && device_type2==0)
        {
            printf("SENSOR: ");
            if (info==0)
                printf("DEL TIPO FLOW\n");
            else if (info==1)
                printf("DEL TIPO TEMP\n");
            else if (info==2)
                printf("DEL TIPO PRESION\n");
            else if (info==3)
                printf("DEL TIPO NIVEL\n");   
        }
        if (device_type1==0 && device_type2==1)
        {
            printf("ACTUADOR: ");
            if (info==0)
                printf("DEL TIPO VALVULA\n");
            else if(info==1)
                printf("DEL TIPO MOTOR\n");    
        }
        if (device_type1==1 && device_type2==1)
        {
            printf("CONCENTRATOR: ");
        }       
    return info;
}
/**
 * @brief Funcion countDevices
 * 
 * @param head Para trabajar con los Header2
 * @param device_type dispositivo a trabajar
 * @param info para identificar la informacion con respecto al dispositivo
 * @param device_types() Funcion emplementada en esta funcion para comparar en el ciclo if (LINEA-221)
 * @return count
 */
// Función para contar dispositivos de un mismo tipo
int countDevices(struct Header2 head, uint8_t device_type, uint8_t info = 255) {
    int count = 0;
    for (int i=0;i<count;i++) {
        if ((head.device_type_NH &&head.device_type_NL) == device_type)
        {
            if (device_type == device_types(1,0,info) || device_type == device_types(0,1,info)) {
                if (head.info == info || info == 255) {
                    ++count;
                }
            } else {
                ++count;
            }
        }
    }
    return count;
}
/**
 * @brief Funcion showDeviceCount()
 * 
 * Función para mostrar la cantidad de equipos de cada tipo
 * 
 * @param head Para trabajar con el Header2
 * @param concentrator_count lo tuve que comentar su linea porque no podia utlizar mi funcion devices_types()
 * @param devices_types Use esta funcion para poder ir mostrando cada tipo de dispositivo con su informacion.
 */

void showDeviceCounts(struct Header2 head) {
    //int cpu_count = countDevices(head, device_types(0,0));
    //int concentrator_count = countDevices(head, device_types(1,1,));
    int flow_sensor_count = countDevices(head,device_types(1,0,0));
    int temp_sensor_count = countDevices(head,device_types(1,0,1));
    int pressure_sensor_count = countDevices(head, device_types(1,0,2));
    int level_sensor_count = countDevices(head,device_types(1,0,3));
    int valve_count = countDevices(head,device_types(0,1,0));
    int motor_count = countDevices(head,device_types(0,1,1));

    //printf("CPU: %d\n", cpu_count);
    //printf("CONCENTRATOR: %d\n", concentrator_count);
    printf("FLOW SENSOR: %d\n", flow_sensor_count);
    printf("TEMP SENSOR: %d\n", temp_sensor_count);
    printf("PRESSURE SENSOR: %d\n", pressure_sensor_count);
    printf("LEVEL SENSOR: %d\n", level_sensor_count);
    printf("VALVE: %d\n", valve_count);
    printf("MOTOR: %d\n", motor_count);
}
int main()
{
    FILE *f=fopen("network_structure.dat","rb");
    int cant;
    fseek(f,0,SEEK_END);
    cant=(ftell(f)/sizeof(struct Header1)); //Cantidad de datos
    printf("\n\nCantidad de paquetes:%d\n\n",cant);
    fseek(f,0,SEEK_SET);
    //Cantidad de paquetes es 39

    //Creamos memoria dinamica para el header1
    uint16_t *data=new uint16_t[cant];
    int result = fread(data, sizeof(uint16_t), cant, f);
    if (result != cant) 
    {
        printf("Error al leer los datos\n");
        delete[] data;
        fclose(f);
        return 1;
    }
 int cant2;
    fseek(f,0,SEEK_END);
    cant2=(ftell(f)/sizeof(struct Header2)); //Cantidad de datos
    printf("\n\nCantidad de paquetes:%d\n\n",cant2);
    fseek(f,0,SEEK_SET);
//Creamos memoria dinamica para el Header2
    uint16_t*data2=new uint16_t[cant2];
    int result2 = fread(data2, sizeof(uint16_t), cant2, f);
    if (result2 != cant2) 
    {
        printf("Error al leer los datos\n");
        delete[] data;
        fclose(f);
        return 1;
    }
//LEEMOS EL 3ER HEADER
    int cant3;
    fseek(f,0,SEEK_END);
    cant3=(ftell(f)/sizeof(struct Header3)); //Cantidad de datos
    printf("\n\nCantidad de paquetes:%d\n\n",cant3);
    fseek(f,0,SEEK_SET);
    struct Header3 *data3=new struct Header3[cant3];
    int result3 = fread(data3, sizeof(Header3), cant2, f);
    if (result != cant3) 
    {
        printf("Error al leer los datos\n");
        delete[] data;
        fclose(f);
        return 1;
    }

    int opcion;
    do
    {
        struct Header1 head=extractHeader1(*data);
        struct Header2 head2=extractHeader2(*data2);
        printf("Seleccione una opcion:\n");
        printf("1.Contar Dispositivos: \n");
        printf("2.Mostrar secuencia de Conexion: \n");
        if (opcion==1)
        {
            uint16_t id;//Creamos variable
            printf("Ingrese el ID del equipo: ");
            scanf("%hu",&id);
            int count=countDevices(head2,id);
            printf("Cantidad de dispositivos conectados a %u:%zu",id,count);
        }
        if(opcion==2)
        {
            uint16_t id;
            printf("Ingrese el ID del equipo: ");
            scanf("%hu",&id);
            showConnectionSequence(head,id);
        }
        
    } while (opcion !=0);
    fclose(f);
    delete[] data;
    delete[]data2;
    delete[]data3;
    return 0;
}
import os
import numpy as np
import cv2
import matplotlib.pyplot as plt
import tensorflow as tf
from tensorflow.keras.preprocessing.image import load_img, img_to_array
from keras.utils import custom_object_scope
import easyocr  # Importar easyocr
import pytesseract as pt

model = tf.keras.models.load_model("object_detection.keras")

def object_detection(path, filename):
    # Leer la imagen
    image = load_img(path) # PIL object
    image = np.array(image,dtype=np.uint8) # 8 bit array (0,255)
    image1 = load_img(path,target_size=(224,224))

    # Redimensionar la imagen para la predicción
    image_arr_224 = img_to_array(image1)/255.0 # Convert to array & normalized
    h,w,d = image.shape
    test_arr = image_arr_224.reshape(1,224,224,3)

    # Hacer la predicción
    coords = model.predict(test_arr)
    
    # Denormalizar las coordenadas
    denorm = np.array([w, w, h, h])  # Para ajustar las coordenadas a la escala original
    coords = coords * denorm  # Multiplicar por las dimensiones de la imagen original
    coords = coords.astype(np.int32)  # Asegurarse de que las coordenadas son enteros
    
    # Obtener las coordenadas de la caja delimitadora
    xmin, xmax, ymin, ymax = coords[0]
    pt1 = (xmin - 20, ymin - 20)
    pt2 = (xmax + 20, ymax + 20)
    print(pt1, pt2)
    
    # Dibujar la caja en la imagen original
    cv2.rectangle(image, pt1, pt2, (0, 255, 0), 3)  # Caja verde con grosor 3
    
    # Convertir a formato BGR para guardar la imagen (OpenCV usa BGR en lugar de RGB)
    image_bgr = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
    
    # Guardar la imagen con la caja delimitadora
    cv2.imwrite('static/predict/{}'.format(filename), image_bgr)
    
    return coords  # Devolver las coordenadas de la caja


def save_text(filename, text):
    name, ext = os.path.splitext(filename)
    with open('static/predict/{}.txt'.format(name), mode='w') as f:
        f.write(text)
    f.close()



def OCR(path, filename):
    # Cargar la imagen
    img = np.array(load_img(path))
    
    # Obtener las coordenadas del objeto detectado
    coords = object_detection(path, filename)
    xmin, xmax, ymin, ymax = coords[0]
    
    # Extraer la región de interés (ROI)
    img = np.array(load_img(path))
    xmin ,xmax,ymin,ymax = coords[0]
    roi = img[ymin-20:ymax+20,xmin-20:xmax+20]
    
    text = pt.image_to_string(roi)
    save_text(filename, text)
    return text

def apply_brightness_contrast(input_img, brightness=0, contrast=0):
    if brightness != 0:
        if brightness > 0:
            shadow = brightness
            highlight = 255
        else:
            shadow = 0
            highlight = 255 + brightness
        alpha_b = (highlight - shadow) / 255
        gamma_b = shadow
        buf = cv2.addWeighted(input_img, alpha_b, input_img, 0, gamma_b)
    else:
        buf = input_img.copy()

    if contrast != 0:
        f = 131 * (contrast + 127) / (127 * (131 - contrast))
        alpha_c = f
        gamma_c = 127 * (1 - f)
        buf = cv2.addWeighted(buf, alpha_c, buf, 0, gamma_c)

    return buf

# model = Model(inputs=model.input, outputs=model.layers[-2].output)
xd = OCR('./TEST.jpg', 'output.jpg')
print(xd)

# for layer in model.layers:
#     if isinstance(layer, CustomScaleLayer):
#         print(f"Scale value: {layer.scale}")

# fig = px.imshow(image)
# fig.update_layout(width=700, height=500, margin=dict(l=10, r=10, b=10, t=10),xaxis_title='Figure 14')
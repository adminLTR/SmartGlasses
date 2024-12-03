from django.contrib.auth.models import User, Group
from rest_framework import viewsets
from rest_framework import permissions
from .serializers import *
from .models import *
from rest_framework.permissions import AllowAny
import random as rd
import base64
import json
from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.http import require_POST
from django.http import JsonResponse
from django.shortcuts import render

class ResidueViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows Residues to be viewed or edited.
    """
    queryset = Capture.objects.all().order_by('-datetime')
    serializer_class = CaptureSerializer
    permission_classes = [AllowAny]


@csrf_exempt
@require_POST
def send_img(request):
    if request.method == 'POST':
        # Verificar si el archivo está presente en la solicitud
        imagen = request.FILES.get('file', None)
        if imagen is not None:
            try:
                # Guardar la imagen directamente en el modelo
                nuevo_registro = Capture()
                nuevo_registro.frame.save(f'{rd.randint(10, 10000)}.jpg', imagen)

                # Verificar si la imagen fue procesada correctamente
                if nuevo_registro.most_confident_label is None or nuevo_registro.confidence is None:
                    return JsonResponse({'error': "Image not detected"}, status=400)

                # Responder con los datos procesados
                return JsonResponse({
                    'most_confident_label': nuevo_registro.most_confident_label,
                    'confidence': nuevo_registro.confidence,
                })

            except Exception as e:
                return JsonResponse({'error': f"Error al procesar la imagen: {str(e)}"}, status=400)

        return JsonResponse({'error': 'No se encontró una imagen en la solicitud'}, status=400)

    return JsonResponse({'error': 'Método no permitido'}, status=405)

# def map_view(request):
#     return render(request, 'api/map.html')
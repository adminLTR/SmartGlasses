from rest_framework import serializers

from .models import *

class CaptureSerializer(serializers.ModelSerializer):
    class Meta:
        model = Capture
        fields = ['datetime', 'frame', 'most_confident_label', 'confidence']

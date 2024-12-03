from django.contrib import admin

from .models import Capture


class CaptureAdmin(admin.ModelAdmin):

    list_display = ("id", 'admin_image', 'datetime', 'most_confident_label', 'confidence')

    ordering = ('-datetime',)

admin.site.register(Capture, CaptureAdmin)
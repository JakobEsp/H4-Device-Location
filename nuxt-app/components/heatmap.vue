<template>
  <div ref="heatmapContainer" style="width: 100%; height: 500px; position: relative;">
    <canvas ref="canvas" style="width: 100%; height: 100%;"></canvas>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted, onBeforeUnmount } from 'vue';

const heatmapContainer = ref();
const canvas = ref();

onMounted(() => {
  if (process.client) {
    setTimeout(() => {
      initHeatmap();
    }, 100);
    
    // Add resize event listener only on client side
    window.addEventListener('resize', handleResize);
  }
});

// Clean up event listener when component is unmounted
onBeforeUnmount(() => {
  if (process.client && window) {
    window.removeEventListener('resize', handleResize);
  }
});

// Separate handler function for resize events
const handleResize = () => {
  if (process.client) {
    initHeatmap();
  }
};

const initHeatmap = () => {
  const container = heatmapContainer.value;
  const canvasEl = canvas.value;
  
  if (!container || !canvasEl) return;
  
  // Set canvas dimensions
  canvasEl.width = container.offsetWidth;
  canvasEl.height = container.offsetHeight;
  
  const ctx = canvasEl.getContext('2d');
  if (!ctx) return;
  
  // Example data points
  const dataPoints = [
    { x: 100, y: 100, value: 80 },
    { x: 200, y: 120, value: 90 },
    { x: 300, y: 150, value: 60 },
    { x: 150, y: 250, value: 70 },
    { x: 250, y: 300, value: 50 },
    { x: 350, y: 350, value: 40 },
    { x: 120, y: 320, value: 65 },
    { x: 220, y: 220, value: 85 },
    { x: 330, y: 180, value: 75 },
    { x: 180, y: 170, value: 95 }
  ];
  
  // Clear canvas
  ctx.clearRect(0, 0, canvasEl.width, canvasEl.height);
  
  // First pass: Create a transparent layer to collect heat
  ctx.globalAlpha = 0.2;
  
  // Draw each data point
  dataPoints.forEach(point => {
    // Calculate radius based on value
    const radius = 30 + (point.value / 100) * 30;
    
    // Create color based on value (red for high, blue for low)
    const intensity = point.value / 100;
    const r = Math.floor(255 * intensity);
    const b = Math.floor(255 * (1 - intensity));
    const g = Math.floor(100 * Math.sin(intensity * Math.PI));
    
    // Draw gradient circle
    const gradient = ctx.createRadialGradient(
      point.x, point.y, 0,
      point.x, point.y, radius
    );
    
    gradient.addColorStop(0, `rgba(${r}, ${g}, ${b}, 0.8)`);
    gradient.addColorStop(0.5, `rgba(${r}, ${g}, ${b}, 0.4)`);
    gradient.addColorStop(1, `rgba(${r}, ${g}, ${b}, 0)`);
    
    ctx.fillStyle = gradient;
    ctx.beginPath();
    ctx.arc(point.x, point.y, radius, 0, 2 * Math.PI);
    ctx.fill();
  });
  
  // Second pass: Apply blur to create heatmap effect
  ctx.globalAlpha = 1.0;
  
  // Create a temporary canvas for the blur effect
  const tempCanvas = document.createElement('canvas');
  tempCanvas.width = canvasEl.width;
  tempCanvas.height = canvasEl.height;
  const tempCtx = tempCanvas.getContext('2d');
  
  if (tempCtx) {
    // Copy current canvas to temp
    tempCtx.drawImage(canvasEl, 0, 0);
    
    // Clear main canvas
    ctx.clearRect(0, 0, canvasEl.width, canvasEl.height);
    
    // Apply blur by drawing multiple times with offset
    for (let i = -2; i <= 2; i++) {
      for (let j = -2; j <= 2; j++) {
        ctx.globalAlpha = 1 / (Math.abs(i) + Math.abs(j) + 1);
        ctx.drawImage(tempCanvas, i, j);
      }
    }
    
    // Final opacity adjustment
    ctx.globalAlpha = 0.8;
    ctx.drawImage(tempCanvas, 0, 0);
  }
};
</script>

<style scoped>
canvas {
  display: block;
}
</style>
<template>
	<div class="w-screen h-screen flex items-center justify-center bg-neutral-100">
		<canvas ref="canvasRef" width="600" height="400" class="border-gray-500"></canvas>
	</div>
</template>

<script setup lang="ts">
import type { WebsocketData } from "../shared/types";

import { onMounted, ref } from "vue";

interface HeatPoint {
	x: number;
	y: number;
	intensity: number; // value from 0 to 1
}

const canvasRef = ref<HTMLCanvasElement | null>(null);

// Function to render the heatmap by drawing radial gradients for each point.
function drawHeatmap(ctx: CanvasRenderingContext2D, points: HeatPoint[]) {
	// Clear previous frame
	ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height);

	// For each point, draw a radial gradient circle.
	points.forEach((point) => {
		// Create a gradient from the center (full intensity) to the edge (transparent)
		const gradient = ctx.createRadialGradient(point.x, point.y, 0, point.x, point.y, 40);
		gradient.addColorStop(0, `rgba(255, 0, 0, ${point.intensity})`);
		gradient.addColorStop(1, "rgba(255, 0, 0, 0)");

		ctx.fillStyle = gradient;
		ctx.beginPath();
		ctx.arc(point.x, point.y, 40, 0, Math.PI * 2);
		ctx.fill();
	});
}

onMounted(() => {
	const canvas = canvasRef.value;
	if (!canvas) return;

	const ctx = canvas.getContext("2d");
	if (!ctx) return;

	// This array will hold points received from the websocket.
	const heatPoints: HeatPoint[] = [];

	// Simulate receiving new data every second.
	// In your app, replace this with your websocket event handler.
	setInterval(() => {
		// For demonstration, generate random location and an intensity from RSSI conversion.
		// You would compute these values using your incoming WebsocketData.
		const x = Math.random() * canvas.width;
		const y = Math.random() * canvas.height;
		const intensity = Math.random(); // intensity between 0 and 1
		heatPoints.push({ x, y, intensity });

		// Optionally, limit the number of points (simulate fading out old data)
		if (heatPoints.length > 100) {
			heatPoints.shift(); // remove the oldest point
		}

		// Render the heatmap with current points
		drawHeatmap(ctx, heatPoints);
	}, 1000);
});
</script>

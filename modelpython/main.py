import math
import torch
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
from transformers import AutoTokenizer, AutoModelForCausalLM

# --- Configuración del Modelo y Puntuación ---
MODEL_NAME = "PlanTL-GOB-ES/gpt2-base-bne"
PPL_TARGET_FOR_SCORE_10 = 30.0
PPL_FLOOR_FOR_SCORE_1 = 700.0
tokenizer = None
model = None

# --- Inicialización de la Aplicación FastAPI ---
app = FastAPI(
    title="API de Evaluación de Congruencia en Español",
    description="Evalúa la coherencia de una frase en español usando un modelo LLM local y devuelve una puntuación de 1 a 10.",
    version="1.0.0"
)


# --- Evento de Inicio: Cargar Modelo ---
@app.on_event("startup")
async def load_model_on_startup():
    """Carga el modelo y tokenizador de Hugging Face al iniciar la API."""
    global tokenizer, model
    print(f"🚀 Iniciando la carga del modelo: {MODEL_NAME}...")
    try:
        tokenizer = AutoTokenizer.from_pretrained(MODEL_NAME)
        model = AutoModelForCausalLM.from_pretrained(MODEL_NAME)
        model.eval()
        print("✅ Modelo y tokenizador cargados exitosamente.")
    except Exception as e:
        print(f"❌ Error crítico al cargar el modelo: {e}")
        tokenizer = None
        model = None


# --- Clases Pydantic para Validación de Entrada/Salida ---
class TextInput(BaseModel):
    texto: str


class CongruenceResponse(BaseModel):
    texto_evaluado: str
    puntuacion_congruencia: float
    perplejidad_calculada: float


# --- Funciones Auxiliares ---
def calculate_perplexity(text_to_evaluate: str, model_loaded, tokenizer_loaded) -> float:
    """Calcula la perplejidad de un texto dado."""
    if not text_to_evaluate.strip():
        return float('inf')

    try:
        inputs = tokenizer_loaded(text_to_evaluate, return_tensors="pt", max_length=512, truncation=True)
        input_ids = inputs.input_ids

        if input_ids.size(1) == 0:
            return float('inf')

        with torch.no_grad():
            outputs = model_loaded(input_ids, labels=input_ids.clone())
            loss = outputs.loss

        perplexity = torch.exp(loss).item()
        return perplexity
    except Exception as e:
        print(f"⚠️ Error calculando perplejidad para '{text_to_evaluate}': {e}")
        return float('inf')


def convert_perplexity_to_score(ppl: float) -> float:
    """Convierte la perplejidad a una puntuación de 1.0 a 10.0."""
    if math.isinf(ppl) or math.isnan(ppl) or ppl >= PPL_FLOOR_FOR_SCORE_1:
        return 1.0
    if ppl <= PPL_TARGET_FOR_SCORE_10:
        return 10.0

    # Mapeo logarítmico-lineal invertido entre los umbrales
    log_ppl = math.log(ppl)
    log_ppl_target = math.log(PPL_TARGET_FOR_SCORE_10)
    log_ppl_floor = math.log(PPL_FLOOR_FOR_SCORE_1)

    score = 1.0 + 9.0 * ((log_ppl_floor - log_ppl) / (log_ppl_floor - log_ppl_target))

    # Asegurar que la puntuación esté entre 1.0 y 10.0 y redondear a 2 decimales
    return round(max(1.0, min(10.0, score)), 2)


# --- Endpoints de la API ---
@app.post("/evaluar_congruencia/", response_model=CongruenceResponse)
async def evaluar_congruencia_texto(item: TextInput):
    """
    Recibe un texto en español y devuelve una puntuación de congruencia (1-10).
    """
    if model is None or tokenizer is None:
        raise HTTPException(status_code=503,
                            detail="El modelo de IA no está disponible o no se pudo cargar. Inténtalo más tarde.")

    if not item.texto or not item.texto.strip():
        raise HTTPException(status_code=400, detail="El campo 'texto' no puede estar vacío.")

    print(f"💬 Evaluando congruencia para: '{item.texto}'")

    perplexity_calculada = calculate_perplexity(item.texto, model, tokenizer)
    puntuacion = convert_perplexity_to_score(perplexity_calculada)

    print(f"Perplejidad: {perplexity_calculada:.2f}, Puntuación 1-10: {puntuacion}")

    return CongruenceResponse(
        texto_evaluado=item.texto,
        puntuacion_congruencia=puntuacion,
        perplejidad_calculada=round(perplexity_calculada, 2) if not (
                    math.isinf(perplexity_calculada) or math.isnan(perplexity_calculada)) else -1.0
    )


@app.get("/")
async def root_path():
    return {"mensaje": "API para Evaluación de Congruencia en Español. Visita /docs para interactuar."}


if __name__ == "__main__":
    import uvicorn

    print(" Iniciando servidor Uvicorn desde main.py... Accede en http://localhost:8000")
    uvicorn.run(app, host="0.0.0.0", port=8000)
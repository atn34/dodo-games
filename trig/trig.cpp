#include "trig.hpp"

namespace {
constexpr int kTaylorSeriesIterations = 15;

constexpr double factorial(double n) {
  if (n == 0) {
    return 1;
  } else {
    return n * factorial(n - 1);
  }
}

constexpr double pow(double x, double p) {
  if (p == 0) {
    return 1;
  } else {
    return x * pow(x, p - 1);
  }
}

constexpr double sin(double x) {
  const int iterations = kTaylorSeriesIterations;
  double result = 0;
  for (int n = 0; n < iterations; ++n) {
    result += pow(-1, n) / factorial(2 * n + 1) * pow(x, 2 * n + 1);
  }
  return result;
}

constexpr int8_t to_table(double x) { return int8_t(sin(x) * 127.f); }

constexpr double tau = 2 * 3.14159265;

constexpr int8_t sin_table[] = {
    to_table(tau * 0. / 256.),   to_table(tau * 1. / 256.),
    to_table(tau * 2. / 256.),   to_table(tau * 3. / 256.),
    to_table(tau * 4. / 256.),   to_table(tau * 5. / 256.),
    to_table(tau * 6. / 256.),   to_table(tau * 7. / 256.),
    to_table(tau * 8. / 256.),   to_table(tau * 9. / 256.),
    to_table(tau * 10. / 256.),  to_table(tau * 11. / 256.),
    to_table(tau * 12. / 256.),  to_table(tau * 13. / 256.),
    to_table(tau * 14. / 256.),  to_table(tau * 15. / 256.),
    to_table(tau * 16. / 256.),  to_table(tau * 17. / 256.),
    to_table(tau * 18. / 256.),  to_table(tau * 19. / 256.),
    to_table(tau * 20. / 256.),  to_table(tau * 21. / 256.),
    to_table(tau * 22. / 256.),  to_table(tau * 23. / 256.),
    to_table(tau * 24. / 256.),  to_table(tau * 25. / 256.),
    to_table(tau * 26. / 256.),  to_table(tau * 27. / 256.),
    to_table(tau * 28. / 256.),  to_table(tau * 29. / 256.),
    to_table(tau * 30. / 256.),  to_table(tau * 31. / 256.),
    to_table(tau * 32. / 256.),  to_table(tau * 33. / 256.),
    to_table(tau * 34. / 256.),  to_table(tau * 35. / 256.),
    to_table(tau * 36. / 256.),  to_table(tau * 37. / 256.),
    to_table(tau * 38. / 256.),  to_table(tau * 39. / 256.),
    to_table(tau * 40. / 256.),  to_table(tau * 41. / 256.),
    to_table(tau * 42. / 256.),  to_table(tau * 43. / 256.),
    to_table(tau * 44. / 256.),  to_table(tau * 45. / 256.),
    to_table(tau * 46. / 256.),  to_table(tau * 47. / 256.),
    to_table(tau * 48. / 256.),  to_table(tau * 49. / 256.),
    to_table(tau * 50. / 256.),  to_table(tau * 51. / 256.),
    to_table(tau * 52. / 256.),  to_table(tau * 53. / 256.),
    to_table(tau * 54. / 256.),  to_table(tau * 55. / 256.),
    to_table(tau * 56. / 256.),  to_table(tau * 57. / 256.),
    to_table(tau * 58. / 256.),  to_table(tau * 59. / 256.),
    to_table(tau * 60. / 256.),  to_table(tau * 61. / 256.),
    to_table(tau * 62. / 256.),  to_table(tau * 63. / 256.),
    to_table(tau * 64. / 256.),  to_table(tau * 65. / 256.),
    to_table(tau * 66. / 256.),  to_table(tau * 67. / 256.),
    to_table(tau * 68. / 256.),  to_table(tau * 69. / 256.),
    to_table(tau * 70. / 256.),  to_table(tau * 71. / 256.),
    to_table(tau * 72. / 256.),  to_table(tau * 73. / 256.),
    to_table(tau * 74. / 256.),  to_table(tau * 75. / 256.),
    to_table(tau * 76. / 256.),  to_table(tau * 77. / 256.),
    to_table(tau * 78. / 256.),  to_table(tau * 79. / 256.),
    to_table(tau * 80. / 256.),  to_table(tau * 81. / 256.),
    to_table(tau * 82. / 256.),  to_table(tau * 83. / 256.),
    to_table(tau * 84. / 256.),  to_table(tau * 85. / 256.),
    to_table(tau * 86. / 256.),  to_table(tau * 87. / 256.),
    to_table(tau * 88. / 256.),  to_table(tau * 89. / 256.),
    to_table(tau * 90. / 256.),  to_table(tau * 91. / 256.),
    to_table(tau * 92. / 256.),  to_table(tau * 93. / 256.),
    to_table(tau * 94. / 256.),  to_table(tau * 95. / 256.),
    to_table(tau * 96. / 256.),  to_table(tau * 97. / 256.),
    to_table(tau * 98. / 256.),  to_table(tau * 99. / 256.),
    to_table(tau * 100. / 256.), to_table(tau * 101. / 256.),
    to_table(tau * 102. / 256.), to_table(tau * 103. / 256.),
    to_table(tau * 104. / 256.), to_table(tau * 105. / 256.),
    to_table(tau * 106. / 256.), to_table(tau * 107. / 256.),
    to_table(tau * 108. / 256.), to_table(tau * 109. / 256.),
    to_table(tau * 110. / 256.), to_table(tau * 111. / 256.),
    to_table(tau * 112. / 256.), to_table(tau * 113. / 256.),
    to_table(tau * 114. / 256.), to_table(tau * 115. / 256.),
    to_table(tau * 116. / 256.), to_table(tau * 117. / 256.),
    to_table(tau * 118. / 256.), to_table(tau * 119. / 256.),
    to_table(tau * 120. / 256.), to_table(tau * 121. / 256.),
    to_table(tau * 122. / 256.), to_table(tau * 123. / 256.),
    to_table(tau * 124. / 256.), to_table(tau * 125. / 256.),
    to_table(tau * 126. / 256.), to_table(tau * 127. / 256.),
    to_table(tau * 128. / 256.), to_table(tau * 129. / 256.),
    to_table(tau * 130. / 256.), to_table(tau * 131. / 256.),
    to_table(tau * 132. / 256.), to_table(tau * 133. / 256.),
    to_table(tau * 134. / 256.), to_table(tau * 135. / 256.),
    to_table(tau * 136. / 256.), to_table(tau * 137. / 256.),
    to_table(tau * 138. / 256.), to_table(tau * 139. / 256.),
    to_table(tau * 140. / 256.), to_table(tau * 141. / 256.),
    to_table(tau * 142. / 256.), to_table(tau * 143. / 256.),
    to_table(tau * 144. / 256.), to_table(tau * 145. / 256.),
    to_table(tau * 146. / 256.), to_table(tau * 147. / 256.),
    to_table(tau * 148. / 256.), to_table(tau * 149. / 256.),
    to_table(tau * 150. / 256.), to_table(tau * 151. / 256.),
    to_table(tau * 152. / 256.), to_table(tau * 153. / 256.),
    to_table(tau * 154. / 256.), to_table(tau * 155. / 256.),
    to_table(tau * 156. / 256.), to_table(tau * 157. / 256.),
    to_table(tau * 158. / 256.), to_table(tau * 159. / 256.),
    to_table(tau * 160. / 256.), to_table(tau * 161. / 256.),
    to_table(tau * 162. / 256.), to_table(tau * 163. / 256.),
    to_table(tau * 164. / 256.), to_table(tau * 165. / 256.),
    to_table(tau * 166. / 256.), to_table(tau * 167. / 256.),
    to_table(tau * 168. / 256.), to_table(tau * 169. / 256.),
    to_table(tau * 170. / 256.), to_table(tau * 171. / 256.),
    to_table(tau * 172. / 256.), to_table(tau * 173. / 256.),
    to_table(tau * 174. / 256.), to_table(tau * 175. / 256.),
    to_table(tau * 176. / 256.), to_table(tau * 177. / 256.),
    to_table(tau * 178. / 256.), to_table(tau * 179. / 256.),
    to_table(tau * 180. / 256.), to_table(tau * 181. / 256.),
    to_table(tau * 182. / 256.), to_table(tau * 183. / 256.),
    to_table(tau * 184. / 256.), to_table(tau * 185. / 256.),
    to_table(tau * 186. / 256.), to_table(tau * 187. / 256.),
    to_table(tau * 188. / 256.), to_table(tau * 189. / 256.),
    to_table(tau * 190. / 256.), to_table(tau * 191. / 256.),
    to_table(tau * 192. / 256.), to_table(tau * 193. / 256.),
    to_table(tau * 194. / 256.), to_table(tau * 195. / 256.),
    to_table(tau * 196. / 256.), to_table(tau * 197. / 256.),
    to_table(tau * 198. / 256.), to_table(tau * 199. / 256.),
    to_table(tau * 200. / 256.), to_table(tau * 201. / 256.),
    to_table(tau * 202. / 256.), to_table(tau * 203. / 256.),
    to_table(tau * 204. / 256.), to_table(tau * 205. / 256.),
    to_table(tau * 206. / 256.), to_table(tau * 207. / 256.),
    to_table(tau * 208. / 256.), to_table(tau * 209. / 256.),
    to_table(tau * 210. / 256.), to_table(tau * 211. / 256.),
    to_table(tau * 212. / 256.), to_table(tau * 213. / 256.),
    to_table(tau * 214. / 256.), to_table(tau * 215. / 256.),
    to_table(tau * 216. / 256.), to_table(tau * 217. / 256.),
    to_table(tau * 218. / 256.), to_table(tau * 219. / 256.),
    to_table(tau * 220. / 256.), to_table(tau * 221. / 256.),
    to_table(tau * 222. / 256.), to_table(tau * 223. / 256.),
    to_table(tau * 224. / 256.), to_table(tau * 225. / 256.),
    to_table(tau * 226. / 256.), to_table(tau * 227. / 256.),
    to_table(tau * 228. / 256.), to_table(tau * 229. / 256.),
    to_table(tau * 230. / 256.), to_table(tau * 231. / 256.),
    to_table(tau * 232. / 256.), to_table(tau * 233. / 256.),
    to_table(tau * 234. / 256.), to_table(tau * 235. / 256.),
    to_table(tau * 236. / 256.), to_table(tau * 237. / 256.),
    to_table(tau * 238. / 256.), to_table(tau * 239. / 256.),
    to_table(tau * 240. / 256.), to_table(tau * 241. / 256.),
    to_table(tau * 242. / 256.), to_table(tau * 243. / 256.),
    to_table(tau * 244. / 256.), to_table(tau * 245. / 256.),
    to_table(tau * 246. / 256.), to_table(tau * 247. / 256.),
    to_table(tau * 248. / 256.), to_table(tau * 249. / 256.),
    to_table(tau * 250. / 256.), to_table(tau * 251. / 256.),
    to_table(tau * 252. / 256.), to_table(tau * 253. / 256.),
    to_table(tau * 254. / 256.), to_table(tau * 255. / 256.)};

} // namespace

int8_t sin(uint8_t angle, uint8_t radius) {
  return (int(radius) * sin_table[angle]) / 127;
}
int8_t cos(uint8_t angle, uint8_t radius) { return sin(64 - angle, radius); }